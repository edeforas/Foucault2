#include "TaskItem.h"

#include <cassert>
///////////////////////////////////////////////////////////////////////
TaskItem::TaskItem(MirrorItem *pItem)
{
    _pItem=pItem;
    _color=Qt::transparent;
}
///////////////////////////////////////////////////////////////////////
void TaskItem::add_item(QGraphicsItem * pItem)
{
    assert(pItem);
    pItem->setParentItem(this);
    _boundingRect=childrenBoundingRect();    
}
///////////////////////////////////////////////////////////////////////
QRectF TaskItem::boundingRect() const
{
    return _boundingRect;
}
///////////////////////////////////////////////////////////////////////
void TaskItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(_color!=Qt::transparent)
        painter->fillRect(option->rect,_color);

    QGraphicsRectItem::paint(painter, option, widget);
}
///////////////////////////////////////////////////////////////////////
void TaskItem::set_background_color(QColor color)
{
    _color=color;
}
///////////////////////////////////////////////////////////////////////
void TaskItem::add_line_tab(string sTitle,vector<double> val,float x,float y, float width)
{
    QGraphicsTextItem* title=new QGraphicsTextItem(sTitle.c_str());
    title->setPos(x,y);
    add_item(title);

    for(unsigned int i=0;i<val.size();i++)
    {
        double dCellStart=x+width*(i+1.)/(val.size()+1.);

        QGraphicsTextItem* item=new QGraphicsTextItem(QString::number(val[i],'g',3));
        item->setPos(dCellStart,y);
        add_item(item);
    }
}
///////////////////////////////////////////////////////////////////////
int TaskItem::block_size() const
{
    return 12;
}
///////////////////////////////////////////////////////////////////////
