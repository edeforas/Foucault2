#include "TaskItem.h"

#include <cassert>
///////////////////////////////////////////////////////////////////////
TaskItem::TaskItem(MirrorItem *pItem, int iBlockSize)
{
    _pItem=pItem;
    _iBlockSize=iBlockSize;
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
void TaskItem::add_line_tab(string sTitle, vector<double> val, float x, float y, float width, bool bDrawRect)
{
    QGraphicsTextItem* title=new QGraphicsTextItem(sTitle.c_str());
    int iSmallVerticalMargin=_iBlockSize*0.25;
    QPen qp(Qt::darkGray);

    //add left margin
    x+=_iBlockSize*0.25;

    title->setPos(x,y);
    add_item(title);

    if(bDrawRect)
    {
        QGraphicsRectItem* itemR=new QGraphicsRectItem(x,y+iSmallVerticalMargin,width,_iBlockSize);
        itemR->setPen(qp);
        add_item(itemR);
    }

    for(unsigned int i=0;i<val.size();i++)
    {
        double dCellStart=x+width*(i+1.)/(val.size()+1.);

        QGraphicsTextItem* item=new QGraphicsTextItem(QString::number(val[i],'g',3));
        item->setPos(dCellStart,y);
        add_item(item);

        if(bDrawRect)
        {
            QGraphicsLineItem* itemL=new QGraphicsLineItem(dCellStart,y+iSmallVerticalMargin,dCellStart,y+_iBlockSize+iSmallVerticalMargin);
            itemL->setPen(qp);
            add_item(itemL);
        }
    }
}
///////////////////////////////////////////////////////////////////////
