#ifndef MirrorCouderMeasure_
#define MirrorCouderMeasure_

#include "MirrorItem.h"
#include <vector>
using namespace std;

class MirrorCouderMeasure : public MirrorItem
{
public:
  MirrorCouderMeasure(Mirror* pParent);

  void set_measure(const vector<double>& vdMeasures, const vector<double>& vdHm, string sAspect);
  void set_measure_meridian( double m ) ;
  double measure_meridian(){ return mirror_meridian;};
  const vector<double>& measures() const;
  const vector<double>& hmx() const; 
  const vector<double>& deltaC() const;
  const vector<double>& lf1000() const;
  const vector<double>& lfro() const;
  const vector<double>& moinsu() const;
  
    double get_lfro() const;
    double get_lambda_wave() const;

    double get_lambda_RMS() const;
    double get_strehl() const;

    string get_aspect() const;
    const vector<double>& surface() const;

    // get the surface as a set of quad splines
    // each control points[i] is beetwen points[i-1] and points[i+1]
    void compute_surface_smooth(vector<double>& pointsX,vector<double>& pointsY) const;

private:
    double find_minimum(double a,double c,double res,double (*fcn)(void *self,double h));

    static double calc_lf1000(void *self, double h);
    static double calc_less_rms(void* self,double curv);
    static double calc_less_ptv(void* self,double curv);

public:
  unsigned int nb_zones() const;
  void set_hmx(vector<double> vdHmx);
  const vector<double>& hmz() const;
  
  const vector<double>& relative_surface() const;
  
  const vector<double>& hm2r() const;
  const vector<double>& hm4f() const;
private:
  vector<double> _vdHmz  ; // really vdHmx withe a first value  either 0 either hole radius.
  vector<double> _vdHm;
  vector<double> _vdHm2R;
  vector<double> _vdHm4F;
  vector<double> _vdRelativeSurface;
  
private:
  vector<double> _measures;
  vector<double> _mesc;
  vector<double> _deltaC;
  vector<double> _lf1000;
  vector<double> _lfro;
  vector<double> _moinsu;
  vector<double> _profil;
  vector<double> _surf;
  double _dLfRoMax;
  double _dStd;
  double _dLambda;
  double _dGlassMax;
  double _dWeightedLambdaRms;
  double _dWeightedStrehl;
  string _sAspect;
  double mirror_meridian;
};

#endif
