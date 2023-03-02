// this file is covered by the General Public License version 2 or later
// please see GPL.html for more details and licensing issues

// copyright Etienne de Foras ( the author )
// mailto: etienne.deforas@gmail.com

#ifndef _Mirror_
#define _Mirror_

#include <vector>
#include <string>
using namespace std;

#define PI 3.14159265359
// Modif P. Crubillé : define MaxZones 
#define MaxZones 64

class MirrorItem;

class Mirror
{
public:
  Mirror();
  virtual ~Mirror();
  void initialize();

  void set_name(string sName);
  const string &name() const;

  double focal() const;
  double radius_curvature() const;
  void set_focal(double dFocal);

  double diameter() const; //mm
  void set_diameter(double dDiameter);

  double hole_diameter() const; // mm 
  void set_hole_diameter(double dHoleDiameter);

  double obstruction_size() const; // mm
  void set_obstruction_size(double dObstructionSize);

  double edge_mask_width() const; //mm
  void set_edge_mask_width(double dEdgeMask);

    double conical() const;
    void set_conical(double dConical);
    bool is_parabolic() const;

    unsigned int nb_zones() const; 
    const vector<double>& hx() const; 
    void set_hx(vector<double> vdHx);

  //    const vector<double>& hz() const;

  /*
  const vector<double>& hmx() const; 
    void set_hmx(vector<double> vdHmx);
  const vector<double>& hmz() const;

    const vector<double>& relative_surface() const;

    const vector<double>& hm2r() const;
    const vector<double>& hm4f() const;
  */
  
    bool is_slit_moving() const;
    void set_is_slit_moving(bool bSlitMoving);

    void add_item(MirrorItem* pItem);
    MirrorItem* get_item(int iPos);
    int nb_item() const;
    void remove_last_item();

    double ro_dif() const;

    static void design_hx_nb_zone(double dDiameter,double dHole,double dFocal,int iNbZone, vector<double>& vdHx);
    static void design_hx_mm_by_zone(double dDiameter,double dHole,double dFocal,double dMMbyZone,vector<double>& vdHx);
    static void design_hx_min_zone_size(double dDiameter, double dHole, double dFocal,double dMinZoneSize, vector<double>& vdHx);

    // ui settings
    void set_show_both_side(bool bShowBothSide);
    bool get_show_both_side() const;

    void set_show_lf_ro(bool bShowLfRo);
    bool get_show_lf_ro() const;

    void set_smooth_curves(bool bSmoothCurves);
    bool get_smooth_curves() const;

    void set_show_colors(bool bShowColors);
    bool get_show_colors() const;

    //0=compact 1=normal 2=details 3=full
    void set_display_mode(int iDisplayMode);
    int get_display_mode() const;

private:
    vector<MirrorItem*> _allItem;
    string _sName;
    double _dFocal;
    double _dDiameter;
  double _dHoleDiameter; // there is nothing so no measure in this circle
  double _dObstructionSize; // measure are possible, but default have no impact
    double _dEdgeMaskWidth;

    double _dConical;
    double _dRoDif;

    //Couder parameters
  vector<double> _vdHx;
  //    vector<double> _vdHz;
  /*
 vector<double> _vdHmx;
  vector<double> _vdHmz; // really vdHmx withe a first value  either 0 either hole radius.
   vector<double> _vdHm;
    vector<double> _vdHm2R;
    vector<double> _vdHm4F;
    vector<double> _vdRelativeSurface;
  */
  
    bool _bIsSlitMoving;

    //ui settings
    bool _bSmoothCurves;
    bool _bShowBothSide;
    bool _bShowLfRo;
    bool _bShowColors;
    int _iDisplayMode; //0=compact 1=normal 2=details 3=full
};

#endif
