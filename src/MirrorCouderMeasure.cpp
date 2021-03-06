#include "MirrorCouderMeasure.h"

#include "Mirror.h"

#include <algorithm>
#include <cmath>
#include <cassert>
using namespace std;

#define RESMES 0.0001
#define YELLOW 560.
#define PI 3.14159265359

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

    unsigned int iNbZone=pMirror->nb_zones();
    _lf1000.resize(iNbZone);
    _lfro.resize(iNbZone);
    _moinsu.resize(iNbZone);
    _profil.resize(iNbZone+1);
    _surf.resize(iNbZone+1);
    _mesc.resize(iNbZone);
    _deltaC.resize(iNbZone);

    for (unsigned int i=0;i<iNbZone;i++)
        _mesc[i]=(vdMeasures[i]-pMirror->hm2r()[i])*(pMirror->is_slit_moving()?2.:1.);

    // computes max and min
    double a=*min_element(_mesc.begin(),_mesc.end());
    double b=*max_element(_mesc.begin(),_mesc.end());

    // search for lf/ro between a end b
    find_minimum(a,b,RESMES,&(calc_lf1000));

    for(unsigned int i=0;i<iNbZone;i++)
        _lfro[i]=_lf1000[i]/pMirror->ro_dif()/1e6;

    _dLfRoMax=*max_element(_lfro.begin(),_lfro.end());

    for(unsigned int i=0;i<iNbZone;i++)
        _moinsu[i]=-_lf1000[i]/pMirror->radius_curvature()*2000.;

    // compute surface profile using slopes
    _profil[0]=0;
    for(unsigned int i=1;i<iNbZone+1;i++)
        _profil[i]=_profil[i-1]+(pMirror->hz()[i]-pMirror->hz()[i-1])*_moinsu[i-1];

    //on calcule le max et le min de la conique a ajuster
    a=*min_element(_moinsu.begin(),_moinsu.end());
    b=*max_element(_moinsu.begin(),_moinsu.end());
    a/=pMirror->hx()[iNbZone-1];
    b/=pMirror->hx()[iNbZone-1];

    a*=2.;
    b*=2.;

    double dReso=1./(pMirror->hx()[iNbZone-1]*pMirror->hx()[iNbZone-1]);

    // compute conique qui minimise le rms
    find_minimum(a,b,dReso,&(calc_less_rms));

    // compute conique qui minimise le ptv
    find_minimum(a,b,dReso,&(calc_less_ptv));

    double dMax=*max_element(_surf.begin(),_surf.end());
    if (dMax!=0.)
        _dLambda=YELLOW/2./dMax;
    else
        _dLambda=9999.;

    _dGlassMax=dMax;

    //compute best parabola (vs RMS)
    _dWeightedLambdaRms=YELLOW/2./_dStd; //   1./(surface std in lambda unit)

    //compute stddev rms
    _dWeightedStrehl=exp(-sqr(2.*PI*1./_dWeightedLambdaRms));
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
double MirrorCouderMeasure::find_minimum(double a,double c,double res,double (*fcn)(void* self,double h))
{
    double b=(a+c)/2.;
    double fb=fcn(this,b);
    while (fabs(a-c)>res)   //dichotomy 1.5
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
    MirrorCouderMeasure* pMes=static_cast<MirrorCouderMeasure*>(self);
    const Mirror* pMirror=pMes->mirror();
    assert(pMirror!=0);

    //compute surface
    for (unsigned int i=0;i<pMirror->nb_zones()+1;i++)
    {
        double dtemp;
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

    // shift between 0 and max-min
    double min=*min_element(pMes->_surf.begin(),pMes->_surf.end());
    for (unsigned int i=0;i<pMirror->nb_zones()+1;i++)
        pMes->_surf[i]-=min;

    // return PTV
    return *max_element(pMes->_surf.begin(),pMes->_surf.end());
}
////////////////////////////////////////////////////////////////////////////////
double MirrorCouderMeasure::calc_less_rms(void* self,double curv)
{
    MirrorCouderMeasure* pMes=static_cast<MirrorCouderMeasure*>(self);
    const Mirror* pMirror=pMes->mirror();
    assert(pMirror!=0);
    unsigned int iNbZone=pMirror->nb_zones();
    double dtemp;

    //compute surface
    for (unsigned int i=0;i<iNbZone+1;i++)
    {
        double denom= 1.-(pMirror->conical()+1.)*sqr(curv*pMirror->hz()[i]);

        if (denom>=0.)
        {
            dtemp=( curv*sqr(pMirror->hz()[i]) ) / ( 1.+sqrt(denom) );
        }
        else
        {
            dtemp=curv*sqr(pMirror->hz()[i]); // bad case
        }

        pMes->_surf[i]=(pMes->_profil[i]-dtemp)/2.;
    }

    //compute mean
    double dM=0.;
    for(unsigned int i=0;i<iNbZone;i++)
        dM+=(pMes->_surf[i]+pMes->_surf[i+1])/2.*pMirror->relative_surface()[i];

    //compute var and stddev
    double dVar=0.;
    for(unsigned int i=0;i<iNbZone;i++)
        dVar+=sqr((pMes->_surf[i]+pMes->_surf[i+1])/2.-dM)*pMirror->relative_surface()[i];

    double dStd=sqrt(dVar);

    pMes->_dStd=dStd;

    return dStd;
}
////////////////////////////////////////////////////////////////////////////////
double MirrorCouderMeasure::calc_lf1000(void* self,double h)
{
    MirrorCouderMeasure* pMes=static_cast<MirrorCouderMeasure*>(self);
    const Mirror* pMirror=pMes->mirror();
    assert(pMirror!=0);

    double minl,maxl;
    for (unsigned int i=0;i<pMirror->nb_zones();i++)
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
double MirrorCouderMeasure::get_lambda_RMS() const
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
void MirrorCouderMeasure::compute_surface_smooth(vector<double>& pointsX,vector<double>& pointsY) const
{
    const vector<double>& hz=mirror()->hz();
    const vector<double>& surf=surface();
    assert(hz.size()==surf.size());
    size_t iNbPoints=hz.size();

    pointsX.resize(iNbPoints+iNbPoints+1); //iNbPoints+1 points and iNbPoints control points
    pointsY.resize(iNbPoints+iNbPoints+1);

    for(unsigned int iZ=1;iZ<iNbPoints-1;iZ++)
    {
        double X1= hz[iZ-1];
        double X2= hz[iZ];
        double X3= hz[iZ+1];

        double X12=(X1+X2)/2.;
        double X23=(X2+X3)/2.;

        double Y1=surf[iZ-1];
        double Y2=surf[iZ];
        double Y3=surf[iZ+1];

        double Y12=(Y1+Y2)/2.;
        double Y23=(Y2+Y3)/2.;

        if(iZ==1)
        {
            //fit a parabolic y=Ax*x+B, that link and slope to X12,Y12
            assert(X2!=X1);
            assert(X12!=0.);
            double dSlope12=(Y2-Y1)/(X2-X1);
            double dA=dSlope12/2./(X12);
            double dB=Y12-dA*(X12)*(X12);

            //find end point and control point
            double Y1Parab=dA*(X1)*(X1)+dB;
            double XM=(X1+X12)/2.;
            double YM=dA*(XM)*(XM)+dB;
            double XC=2*XM-(X1+X12)/2.;
            double YC=2*YM-(Y1Parab+Y12)/2.;

            //draw
            pointsX[0]=X1;
            pointsY[0]=Y1Parab;
            pointsX[1]=XC;
            pointsY[1]=YC;
            pointsX[2]=X12;
            pointsY[2]=Y12;
        }

        pointsX[2*iZ+1]=X2;
        pointsY[2*iZ+1]=Y2;

        pointsX[2*iZ+2]=X23;
        pointsY[2*iZ+2]=Y23;

        if((unsigned int)iZ==hz.size()-2) //last point : fit a line in a quad
        {
            pointsX[2*iZ+3]=(X23+X3)/2.;
            pointsY[2*iZ+3]=(Y23+Y3)/2.;
            pointsX[2*iZ+4]=X3;
            pointsY[2*iZ+4]=Y3;
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
