#include "MirrorItem.h"
#include "MirrorSurface.h"
#include "Mirror.h"

#include <cassert>

MirrorItem::MirrorItem()
{
    _pParent=0;
}

MirrorItem::~MirrorItem()
{ }

string MirrorItem::type() const
{
    return _sType;
}

const MirrorSurface& MirrorItem::surface() const
{
    return _surface;
}

Mirror* MirrorItem::mirror()
{
    return _pParent;
}

void MirrorItem::set_mirror(Mirror* pMirror)
{
    _pParent=pMirror;
}
