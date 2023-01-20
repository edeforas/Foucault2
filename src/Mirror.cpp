#include "Mirror.h"
#include "MirrorItem.h"

#include <math.h>
inline double sqr(double a) { return a*a; }

//////////////////////////////////////////////////////////////////////////////
Mirror::Mirror()
{
    _dDiameter=0.;
    _dHoleDiameter=0.;
    _dObstructionSize=0.;
    _dEdgeMaskWidth=0.;
    _dFocal=0.;
    _dConical=-1.;
    _bIsSlitMoving=false;
    _dRoDif=0;
    _bShowLfRo=false;

    _bShowBothSide=false;
    _bSmoothCurves=false;

    _bShowColors=true;
    _iDisplayMode=1; //0=compact 1=normal 2=details
}
//////////////////////////////////////////////////////////////////////////////
Mirror::~Mirror()
{
    for(unsigned int i=0;i<_allItem.size();i++)
        delete _allItem[i];
}
//////////////////////////////////////////////////////////////////////////////
void Mirror::add_item(MirrorItem* pItem)
{
    pItem->set_mirror(this);
    _allItem.push_back(pItem);
}
//////////////////////////////////////////////////////////////////////////////
int Mirror::nb_item() const
{
    return (int)(_allItem.size());
}
//////////////////////////////////////////////////////////////////////////////
MirrorItem* Mirror::get_item(int iPos)
{
    return _allItem.at(iPos);
}
//////////////////////////////////////////////////////////////////////////////
void Mirror::set_name(string sName)
{
    _sName=sName;
}
//////////////////////////////////////////////////////////////////////////////
const string& Mirror::name() const
{
    return _sName;
}
//////////////////////////////////////////////////////////////////////////////
double Mirror::focal() const
{
    return _dFocal;
}
//////////////////////////////////////////////////////////////////////////////
double Mirror::radius_curvature() const
{
    return 2.*_dFocal;
}
//////////////////////////////////////////////////////////////////////////////
void Mirror::set_focal(double dFocal)
{
    _dFocal=dFocal;
}
//////////////////////////////////////////////////////////////////////////////
double Mirror::diameter() const
{
    return _dDiameter;
}
//////////////////////////////////////////////////////////////////////////////
void Mirror::set_diameter(double dDiameter)
{
    _dDiameter=dDiameter;
}
//////////////////////////////////////////////////////////////////////////////
double Mirror::hole_diameter() const
{
    return _dHoleDiameter;
}

void Mirror::set_hole_diameter(double dHoleDiameter)
{
    _dHoleDiameter=dHoleDiameter;
}
//////////////////////////////////////////////////////////////////////////////
double Mirror::obstruction_size() const
{
    return _dObstructionSize;
}

void Mirror::set_obstruction_size(double dObstructionSize)
{
    _dObstructionSize=dObstructionSize;
}
//////////////////////////////////////////////////////////////////////////////
double Mirror::edge_mask_width() const
{
    return _dEdgeMaskWidth;
}

void Mirror::set_edge_mask_width(double dEdgeMask)
{
    _dEdgeMaskWidth=dEdgeMask;
}
//////////////////////////////////////////////////////////////////////////////
double Mirror::conical() const
{
    return _dConical;
}
//////////////////////////////////////////////////////////////////////////////
void Mirror::set_conical(double dConical)
{
    _dConical=dConical;
}
//////////////////////////////////////////////////////////////////////////////
bool Mirror::is_parabolic() const
{
    return _dConical==-1.;
}
//////////////////////////////////////////////////////////////////////////////
double Mirror::ro_dif() const
{
    return _dRoDif;
}
//////////////////////////////////////////////////////////////////////////////
unsigned int Mirror::nb_zones() const
{
    return (int)(_vdHx.size());
}
//////////////////////////////////////////////////////////////////////////////
const vector<double>& Mirror::hx() const
{
    return _vdHx;
}

//////////////////////////////////////////////////////////////////////////////
void Mirror::set_hx(vector<double> vdHx)
{
    _vdHx=vdHx;
}
/*
const vector<double>& Mirror::hz() const
{
    return _vdHz;
}
*/
//////////////////////////////////////////////////////////////////////////////
bool Mirror::is_slit_moving() const
{
    return _bIsSlitMoving;
}
//////////////////////////////////////////////////////////////////////////////
void Mirror::set_is_slit_moving(bool bSlitMoving)
{
    _bIsSlitMoving=bSlitMoving;
}
//////////////////////////////////////////////////////////////////////////////
void Mirror::initialize()
{
    //    int iNbZone=(int)(hx().size());
    double dRay=2.*_dFocal;

    double dYellow=560.*1.e-9;
    _dRoDif=1.22*dYellow*dRay/_dDiameter/2.; //unit? TODO

    //compute Hz (not saved)
    //    _vdHz.resize(iNbZone+1);
    /*
    _vdHmx.resize(iNbZone+1);
    _vdHmz.resize(iNbZone+1);
    _vdHmz[0]=_dHoleDiameter/2.;
    for(int i=0;i<iNbZone;i++)
        _vdHmz[i+1]=_vdHmx[i];

    //compute Hm
    _vdHm.resize(iNbZone);
    for(int i=0;i<iNbZone;i++)
    {
        _vdHm[i]= _vdHmx[i];
    }

    //compute Hm2R
    _vdHm2R.resize(iNbZone);
    for(int i=0;i<iNbZone;i++)
    {
        if (_bIsSlitMoving)
            _vdHm2R[i]=-_dConical*sqr(_vdHm[i])/2./dRay;
        else
            _vdHm2R[i]=-_dConical*( sqr(_vdHm[i])/dRay + sqr(sqr(_vdHm[i])) /2. /dRay/sqr(dRay));
    }

    //compute Hm4F
    _vdHm4F.resize(iNbZone);
    for (int i=0;i<iNbZone;i++)
        _vdHm4F[i]=_vdHm[i]/dRay/2.;

    //calcule les surfaces relatives
    _vdRelativeSurface.resize(iNbZone);
    double dSum=0.; double current = _vdHmz[0]; // 0 or hole radius.
    for(int i=0;i<(iNbZone-1);i++)
    {
      double next_c;
      next_c = (_vdHmz[i+1]+_vdHmz[i+2])/2;
      _vdRelativeSurface[i]=sqr(next_c)-sqr(current);
    //        _vdRelativeSurface[i]=sqr(_vdHz[i+1])-sqr(_vdHz[i]);
      current = next_c;
      dSum+=_vdRelativeSurface[i];
    }
_vdRelativeSurface[iNbZone -1]=sqr( _dDiameter/2 )-sqr(current);
      dSum+=_vdRelativeSurface[iNbZone -1];

for(int i=0;i<iNbZone;i++)
    {
        _vdRelativeSurface[i]=_vdRelativeSurface[i]/dSum; // *iNbZone;
    }
*/
}
//////////////////////////////////////////////////////////////////////////////
void Mirror::design_hx_nb_zone(double dDiameter,double dHole,double dFocal,int iNbZone, vector<double>& vdHx)
{
    double dRayCurv=2.*dFocal;
    double dRadius=dDiameter/2.;

    if (iNbZone> MaxZones )
        iNbZone= MaxZones ;

    if (iNbZone<3)
        iNbZone=3;

    vdHx.resize(iNbZone);

    //recalcul du tirage par zone
    double dTir=(sqr(dRadius)-sqr(dHole))/dRayCurv/iNbZone;

    //last zone hx=rayon
    vdHx[iNbZone-1]=dRadius;
    for (int i=iNbZone-2;i>-1;i--)
        vdHx[i]=sqrt(sqr(vdHx[i+1])-dRayCurv*dTir);
}
//////////////////////////////////////////////////////////////////////////////
void Mirror::design_hx_mm_by_zone(double dDiameter, double dHole, double dFocal, double dMMbyZone, vector<double>& vdHx)
{
    double dRayCurv=2.*dFocal;
    double dRadius=dDiameter/2.;

    int iNbZone=int( .5+( (double(sqr(dRadius)-sqr(dHole)))/dRayCurv) / dMMbyZone );

    design_hx_nb_zone(dDiameter,dHole,dFocal,iNbZone,vdHx);
}
//////////////////////////////////////////////////////////////////////////////
void Mirror::design_hx_min_zone_size(double dDiameter, double dHole, double dFocal,double dMinZoneSize, vector<double>& vdHx)
{
    double dRayCurv=2.*dFocal;
    double dRadius=dDiameter/2.;

    double val=(dRadius-dMinZoneSize); //hx de l'avant derniere zone
    double dTir=(sqr(dRadius)-sqr(val))/dRayCurv; //calc du tirage correspondant
    int iNbZone=(int)( ( (double(sqr(dRadius)-sqr(dHole)))/dRayCurv) / dTir );

    design_hx_nb_zone(dDiameter,dHole,dFocal,iNbZone,vdHx);
}
//////////////////////////////////////////////////////////////////////////////
void Mirror::set_show_both_side(bool bShowBothSide)
{
    _bShowBothSide=bShowBothSide;
}

bool Mirror::get_show_both_side() const
{
    return _bShowBothSide;
}
//////////////////////////////////////////////////////////////////////////////
void Mirror::set_show_lf_ro(bool bShowLfRo)
{
    _bShowLfRo = bShowLfRo;
}

bool Mirror::get_show_lf_ro() const
{
    return _bShowLfRo;
}
//////////////////////////////////////////////////////////////////////////////
void Mirror::set_smooth_curves(bool bSmoothCurves)
{
    _bSmoothCurves=bSmoothCurves;
}
//////////////////////////////////////////////////////////////////////////////
bool Mirror::get_smooth_curves() const
{
    return _bSmoothCurves;
}
//////////////////////////////////////////////////////////////////////////////
void Mirror::set_show_colors(bool bShowColors)
{
    _bShowColors=bShowColors;
}
//////////////////////////////////////////////////////////////////////////////
bool Mirror::get_show_colors() const
{
    return _bShowColors;
}
//////////////////////////////////////////////////////////////////////////////
void Mirror::set_display_mode(int iDisplayMode)    //0=compact 1=normal 2=details 3=full
{
    _iDisplayMode=iDisplayMode;
}
//////////////////////////////////////////////////////////////////////////////
int Mirror::get_display_mode() const
{
    return _iDisplayMode;
}
//////////////////////////////////////////////////////////////////////////////
void Mirror::remove_last_item()
{
    if(!_allItem.empty())
    {
        MirrorItem* pI=_allItem.back();
        delete pI;
        _allItem.pop_back(); //todo check delete is not called
    }
}
//////////////////////////////////////////////////////////////////////////////
