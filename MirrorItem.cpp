#include "MirrorItem.h"
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

Mirror* MirrorItem::mirror()
{
    return _pParent;
}

void MirrorItem::set_mirror(Mirror* pMirror)
{
    _pParent=pMirror;
}
