#include "MirrorItem.h"
#include "Mirror.h"

#include <ctime>
#include <QDateTime>
#include <QObject>

///////////////////////////////////////////////////////////
MirrorItem::MirrorItem()
{
    _pParent=0;
    _uiWhen=0;
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
void MirrorItem::set_when(unsigned int uiWhen)
{
    _uiWhen=uiWhen;
}
///////////////////////////////////////////////////////////
unsigned int MirrorItem::when() const
{
    return _uiWhen;
}
///////////////////////////////////////////////////////////
string MirrorItem::when_as_text() const
{
    if(_uiWhen)
    {
        QDateTime qdt=QDateTime::fromMSecsSinceEpoch(_uiWhen*1000);
        QString qs=QObject::tr("When: ")+qdt.toString(Qt::SystemLocaleLongDate);
        return qs.toStdString();
    }
    else
        return "";
}
///////////////////////////////////////////////////////////
