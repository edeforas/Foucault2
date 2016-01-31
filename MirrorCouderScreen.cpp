#include "MirrorCouderScreen.h"

MirrorCouderScreen::MirrorCouderScreen()
{
    _sType="MirrorCouderScreen";
    _bIsMovingLight=false;
}

vector<double>& MirrorCouderScreen::hx()
{
    return _hx;
}

bool& MirrorCouderScreen::is_moving_light()
{
    return _bIsMovingLight;
}
