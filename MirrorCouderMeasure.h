#ifndef MirrorCouderMeasure_
#define MirrorCouderMeasure_

#include "MirrorItem.h"
#include <vector>
using namespace std;

class MirrorCouderMeasure : public MirrorItem
{
public:
    MirrorCouderMeasure(Mirror* pParent);

    void set_measure(const vector<double> &vdMeasures, string sAspect);
    const vector<double>& measures() const;
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

private:
    double dichotomy(double a,double c,double res,double (*fcn)(void *self,double h));

    static double calc_lf1000(void *self, double h);
    static double calc_less_rms(void* self,double curv);
    static double calc_less_ptv(void* self,double curv);

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
};

#endif
