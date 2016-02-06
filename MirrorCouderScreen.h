#ifndef MirrorCouderScreen_
#define MirrorCouderScreen_

#include "MirrorItem.h"
#include <vector>
using namespace std;

class MirrorCouderScreen : public MirrorItem
{
public:
    MirrorCouderScreen();

    vector<double>& hx();
    bool& is_moving_light();

private:
    vector<double> _hx;
    bool _bIsMovingLight;
};

#endif
