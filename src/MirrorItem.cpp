#include "MirrorItem.h"
#include "Mirror.h"

#include <QDateTime>
#include <QObject>

///////////////////////////////////////////////////////////
MirrorItem::MirrorItem()
{
    _pParent=0;
    _lWhen=0;
}
///////////////////////////////////////////////////////////
MirrorItem::~MirrorItem()
{ }
///////////////////////////////////////////////////////////
string MirrorItem::type() const
{
    return _sType;
}
///////////////////////////////////////////////////////////
const Mirror* MirrorItem::mirror() const
{
    return _pParent;
}
///////////////////////////////////////////////////////////
void MirrorItem::set_mirror(Mirror* pMirror)
{
    _pParent=pMirror;
}
///////////////////////////////////////////////////////////
void MirrorItem::set_when(long lWhen)
{
    _lWhen=lWhen;
}
///////////////////////////////////////////////////////////
long MirrorItem::when() const
{
    return _lWhen;
}
///////////////////////////////////////////////////////////
string MirrorItem::when_as_text() const
{
    if(_lWhen!=0)
    {
        QDateTime qdt=QDateTime::fromSecsSinceEpoch(_lWhen);
        QString qs=QObject::tr("When: ")+qdt.toString();
        return qs.toStdString();
    }
    else
        return "";
}
///////////////////////////////////////////////////////////
