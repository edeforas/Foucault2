#include "TimelineScene.h"
#include "TaskItem.h"

#include <QGraphicsView>
#include <QGraphicsSceneWheelEvent>
#include <QKeyEvent>
#include <QTimer>

#include "Mirror.h"
#include "MirrorItem.h"
#include "TaskItem.h"
#include "TaskItemNewMirror.h"
#include "TaskItemComment.h"
#include "TaskItemWork.h"
#include "TaskItemCouderMeasure.h"
#include "TaskItemUnmaskedMeasure.h"

#include <cassert>
///////////////////////////////////////////////////////////////////////////////
TimelineScene::TimelineScene()
{
    _pM=0;

    QFontInfo qfi(font());
    _iFontHeight=qfi.pixelSize()*1.25;
    _iSceneBorder=3*_iFontHeight;
}
///////////////////////////////////////////////////////////////////////////////
void TimelineScene::add_item(TaskItem* pi)
{
    double dOldY=0;
    if(!_vti.empty())
    {
        TaskItem* pLastItem=_vti[_vti.size()-1];
        dOldY=pLastItem->boundingRect().bottom()+pLastItem->pos().y();
    }

    pi->setRect(pi->boundingRect());
    pi->setPos(0,dOldY);
    _vti.push_back(pi);
    addItem(pi);

    //update scene rect to add borders
    QRectF qr=itemsBoundingRect();
    QRectF qrWithBorder(qr.left()-_iSceneBorder,qr.top()-_iSceneBorder,qr.width()+2*_iSceneBorder,qr.height()+_iSceneBorder);
    setSceneRect(qrWithBorder);
}
///////////////////////////////////////////////////////////////////////////////
void TimelineScene::wheelEvent(QGraphicsSceneWheelEvent* wheelEvent)
{
    if((wheelEvent->modifiers().testFlag(Qt::ControlModifier)))
    {
        QGraphicsView* v=views()[0];
        if (wheelEvent->delta()>0)
            v->scale(1.25,1.25);
        else
            v->scale(0.8,0.8);

        wheelEvent->accept();
    }
    else
        QGraphicsScene::wheelEvent(wheelEvent);
}
///////////////////////////////////////////////////////////////////////////////
void TimelineScene::set_mirror(Mirror* pM)
{
    _pM=pM;
    clear(); //todo check
    update_items(-1);

    //todo clear viewport
}
///////////////////////////////////////////////////////////////////////////////
void TimelineScene::update_items(int iFirstItem)
{
    if(_pM==0)
    {
        clear();
        _vti.clear();
        return;
    }

    if(iFirstItem==-1)
    {
        _vti.clear();
        clear();
        if(_pM->diameter()==0) //TODO
            return;

        MirrorItem mi;
        mi.set_mirror(_pM);

        TaskItemNewMirror* ptinm=new TaskItemNewMirror(&mi,_iFontHeight);
        add_item(ptinm);
        iFirstItem=0;
    }

    for(int i=iFirstItem;i<_pM->nb_item();i++)
    {
        TaskItem* pti=create_item(_pM->get_item(i));
        add_item(pti);
    }
}
///////////////////////////////////////////////////////////////////////////////
TaskItem* TimelineScene::create_item(MirrorItem* rmi)
{
    if(rmi->type()=="MirrorComment")
    {
        return new TaskItemComment(rmi,_iFontHeight);
    }

    if(rmi->type()=="MirrorWork")
    {
        return new TaskItemWork(rmi,_iFontHeight);
    }

    if(rmi->type()=="MirrorUnmaskedMeasure")
    {
        return new TaskItemUnmaskedMeasure(rmi,_iFontHeight);
    }

    if(rmi->type()=="MirrorCouderMeasure")
    {
        return new TaskItemCouderMeasure(rmi,_iFontHeight);
    }

    return 0;
}
///////////////////////////////////////////////////////////////////////////////
void TimelineScene::ensure_visible(int iItem)
{
    QGraphicsView* pView=views()[0];
    if(iItem>=0)
    {
        assert(iItem+1<(int)_vti.size());
        TaskItem* pItem=_vti[iItem+1];
        pView->ensureVisible(pItem);
    }
    else
    {
        QRectF rectTotal=sceneRect();
        QRect rectWidget=pView->rect();

        if(rectWidget.width()!=0)
        {
            double dRatio=rectWidget.height()/(double)rectWidget.width();
            double newWidth=rectTotal.width();
            double newHeight=rectTotal.width()*dRatio;
            QRectF finalRect(rectTotal.topLeft(),QPointF(newWidth,newHeight));
            pView->fitInView(finalRect,Qt::KeepAspectRatioByExpanding);
            pView->centerOn(rectTotal.topLeft());
        }
    }
}
///////////////////////////////////////////////////////////////////////////////
void TimelineScene::ensure_last_visible()
{
    QGraphicsView* pView=views()[0];
    if(_vti.size()!=0)
    {
        TaskItem* pItem=_vti.back();
        pView->ensureVisible(pItem);
    }
}
///////////////////////////////////////////////////////////////////////////////
void TimelineScene::zoom_in()
{
    QGraphicsView* v=views()[0];
    v->scale(1.25,1.25);
}
///////////////////////////////////////////////////////////////////////////////
void TimelineScene::zoom_out()
{
    QGraphicsView* v=views()[0];
    v->scale(0.8,0.8);
}
///////////////////////////////////////////////////////////////////////////////
const vector<TaskItem*>& TimelineScene::items() const
{
    return _vti;
}
///////////////////////////////////////////////////////////////////////////////
