#include "MirrorCouderMeasure.h"

#include "Mirror.h"

#include <algorithm>
#include <cmath>
#include <cassert>
using namespace std;

#define RESMES 0.0001
#define YELLOW 560.

inline double sqr(double a) { return a*a; }

MirrorCouderMeasure::MirrorCouderMeasure(Mirror *pParent)
{
    _sType="MirrorCouderMeasure";
    _dLfRoMax=0.;
    _dStd=0.;
    _dLambda=0.;
    _dGlassMax=0.;
    _dWeightedLambdaRms=0.;
    _dWeightedStrehl=0.;
    set_mirror(pParent);
}
////////////////////////////////////////////////////////////////////////////////
void MirrorCouderMeasure::set_measure(const vector<double>& vdMeasures,string sAspect="")
{
    const Mirror* pMirror=mirror();
    _measures=vdMeasures;

    _sAspect=sAspect;
    assert(pMirror);

    int iNbZone=pMirror->nb_zones();
    _lf1000.resize(iNbZone);
    _lfro.resize(iNbZone);
    _moinsu.resize(iNbZone);
    _profil.resize(iNbZone+1);
    _surf.resize(iNbZone+1);
    _mesc.resize(iNbZone);
    _deltaC.resize(iNbZone);

    for (int i=0;i<iNbZone;i++)
        _mesc[i]=(vdMeasures[i]-pMirror->hm2r()[i])*(pMirror->is_slit_moving()?2.:1.);

    //on calcules la mesure max et min
    double a=*min_element(_mesc.begin(),_mesc.end());
    double b=*max_element(_mesc.begin(),_mesc.end());

    //et on cherche le lf/ro entre ces valeurs
    dichotomy(a,b,RESMES,&(calc_lf1000));

    for (int i=0;i<iNbZone;i++)
        _lfro[i]=_lf1000[i]/pMirror->ro_dif()/1e6;

    _dLfRoMax=*max_element(_lfro.begin(),_lfro.end());

    for (int i=0;i<iNbZone;i++)
        _moinsu[i]=-_lf1000[i]/pMirror->radius_curvature()*2000.; //TODO was ray

    //on calcule le profil de la surface grace aux pentes
    _profil[0]=0;
    for (int i=1;i<iNbZone+1;i++)
        _profil[i]=_profil[i-1]+(pMirror->hz()[i]-pMirror->hz()[i-1])*_moinsu[i-1];

    //on calcule le max et le min de la conique a ajuster
    a=*min_element(_moinsu.begin(),_moinsu.end());
    b=*max_element(_moinsu.begin(),_moinsu.end());
    a/=pMirror->hx()[iNbZone-1];
    b/=pMirror->hx()[iNbZone-1];

    a*=2.; //essai
    b*=2.;

    double dReso=1./(pMirror->hx()[iNbZone-1]*pMirror->hx()[iNbZone-1]);

    // on cherche la conique qui minimise le rms
    dichotomy(a,b,dReso,&(calc_less_rms));

    //et on cherche la conique qui minimise le ptv
    dichotomy(a,b,dReso,&(calc_less_ptv));

    double dMax=*max_element(_surf.begin(),_surf.end());
    if (dMax!=0.)
        _dLambda=YELLOW/2./dMax;
    else
        _dLambda=9999.;

    _dGlassMax=dMax;

    //calcul de la meilleur parabole au sens RMS

    _dWeightedLambdaRms=YELLOW/2./_dStd; //   1./(ecart type de la surface en unite lambda)

    //calcul de l'ecart type rms
    _dWeightedStrehl=exp(-sqr(2.*M_PI*1./_dWeightedLambdaRms));
}
////////////////////////////////////////////////////////////////////////////////
const vector<double>& MirrorCouderMeasure::measures() const
{
    return _measures;
}
////////////////////////////////////////////////////////////////////////////////
const vector<double>& MirrorCouderMeasure::deltaC() const
{
    return _deltaC;
}
////////////////////////////////////////////////////////////////////////////////
const vector<double>& MirrorCouderMeasure::lf1000() const
{
    return _lf1000;
}
////////////////////////////////////////////////////////////////////////////////
const vector<double>& MirrorCouderMeasure::lfro() const
{
    return _lfro;
}
////////////////////////////////////////////////////////////////////////////////
const vector<double>& MirrorCouderMeasure::moinsu() const
{
    return _moinsu;
}
////////////////////////////////////////////////////////////////////////////////
const vector<double>& MirrorCouderMeasure::surface() const
{
    return _surf;
}
////////////////////////////////////////////////////////////////////////////////
double MirrorCouderMeasure::dichotomy(double a,double c,double res,double (*fcn)(void* self,double h))
{
    double b=(a+c)/2.;
    double fb=fcn(this,b);
    while (fabs(a-c)>res)   //dichotomie 1.5
    {  
	double m1=(a+b)/2.;
        double fm1=fcn(this,m1);
        if (fabs(fm1)<fabs(fb))
        {
            c=b;
            b=m1;
            fb=fm1;
        }
        else
        {
            double m2=(b+c)/2.;
            double fm2=fcn(this,m2);
            if (fabs(fm2)<fabs(fb))
            {
                a=b;
                b=m2;
                fb=fm2;
            }
            else
            {
                a=m1;
                b=m2;
                fb=fm2;
            }
        }
    }
    return b;
}
////////////////////////////////////////////////////////////////////////////////
double MirrorCouderMeasure::calc_less_ptv(void* self,double curv)
{
    MirrorCouderMeasure* pMes=(MirrorCouderMeasure*)self;
    const Mirror* pMirror=pMes->mirror();
    assert(pMirror!=0);
    int iNbZone=pMirror->nb_zones();

    double dtemp;

    //on calcule la surface

    double min;
    for (int i=0;i<iNbZone+1;i++)
    {
        double denom= 1.-(pMirror->conical()+1.)*sqr(curv*pMirror->hz()[i]);

        if (denom>=0.)
        {
            dtemp=( curv*sqr(pMirror->hz()[i]) ) / ( 1.+sqrt(denom) );
        }
        else
        {
            dtemp=curv*sqr(pMirror->hz()[i]); // cas degrade
        }

        pMes->_surf[i]=(pMes->_profil[i]-dtemp)/2.;
    }
    //on la decale entre 0 et max-min
    min=*min_element(pMes->_surf.begin(),pMes->_surf.end());
    for (int i=0;i<iNbZone+1;i++)
        pMes->_surf[i]-=min;

    // on retourne le PTV
    return *max_element(pMes->_surf.begin(),pMes->_surf.end());
}
////////////////////////////////////////////////////////////////////////////////
double MirrorCouderMeasure::calc_less_rms(void* self,double curv)
{
    MirrorCouderMeasure* pMes=(MirrorCouderMeasure*)self;
    const Mirror* pMirror=pMes->mirror();
    assert(pMirror!=0);
    int iNbZone=pMirror->nb_zones();
    double dtemp;

    //on calcule la surface
    for (int i=0;i<iNbZone+1;i++)
    {
        double denom= 1.-(pMirror->conical()+1.)*sqr(curv*pMirror->hz()[i]);

        if (denom>=0.)
        {
            dtemp=( curv*sqr(pMirror->hz()[i]) ) / ( 1.+sqrt(denom) );
        }
        else
        {
            dtemp=curv*sqr(pMirror->hz()[i]); // cas degrade
        }

        pMes->_surf[i]=(pMes->_profil[i]-dtemp)/2.;
    }

    //calcule la moyenne
    double dM=0.;
    for(int i=0;i<iNbZone+1;i++)
        dM+=pMes->_surf[i];

    dM/=(iNbZone+1);

    //calcule la variance et l'ecart type
    double dVar=0.;
    for(int i=0;i<iNbZone+1;i++)
        dVar+=sqr(pMes->_surf[i]-dM)*pMirror->relative_surface()[i];

    dVar/=(iNbZone+1);
    double dStd=sqrt(dVar);

    pMes->_dStd=dStd;

    return dStd;
}
////////////////////////////////////////////////////////////////////////////////
double MirrorCouderMeasure::calc_lf1000(void* self,double h)
{
    MirrorCouderMeasure* pMes=(MirrorCouderMeasure*)self;
    const Mirror* pMirror=pMes->mirror();
    assert(pMirror!=0);
    int iNbZone=pMirror->nb_zones();

    double minl,maxl;
    for (int i=0;i<iNbZone;i++)
    {
        pMes->_deltaC[i]=pMes->_mesc[i]-h;
        pMes->_lf1000[i]=1000.*pMes->_deltaC[i]*pMirror->hm4f()[i];
    }

    minl=*min_element(pMes->_lf1000.begin(),pMes->_lf1000.end());
    maxl=*max_element(pMes->_lf1000.begin(),pMes->_lf1000.end());
    return(maxl+minl);
}
////////////////////////////////////////////////////////////////////////////////
double MirrorCouderMeasure::get_lfro() const
{
    return _dLfRoMax;
}
////////////////////////////////////////////////////////////////////////////////
double MirrorCouderMeasure::get_lambda_wave() const
{
    return _dLambda;
}
////////////////////////////////////////////////////////////////////////////////
double MirrorCouderMeasure::get_RMS() const
{
    return _dWeightedLambdaRms;
}
////////////////////////////////////////////////////////////////////////////////
double MirrorCouderMeasure::get_strehl() const
{
    return _dWeightedStrehl;
}
////////////////////////////////////////////////////////////////////////////////
string MirrorCouderMeasure::get_aspect() const
{
    return _sAspect;
}
////////////////////////////////////////////////////////////////////////////////


