#ifndef _MirrorItem_
#define _MirrorItem_

#include <string>
using namespace std;

#include "MirrorSurface.h"

class Mirror;

class MirrorItem
{
public:
    MirrorItem();
    virtual ~MirrorItem();

    void set_mirror(Mirror* pMirror);
    Mirror* mirror();

    const MirrorSurface& surface() const;

    string type() const;

protected:
    string _sType;
    MirrorSurface _surface;
    Mirror* _pParent;
};

#endif
