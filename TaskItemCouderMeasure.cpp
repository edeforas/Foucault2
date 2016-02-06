#include "TaskItemCouderMeasure.h"
#include <QPen>
#include "Mirror.h"
#include "MirrorCouderMeasure.h"
#include <cassert>

TaskItemCouderMeasure::TaskItemCouderMeasure(MirrorItem* pItem):TaskItem(pItem)
{
    Mirror* pM=pItem->mirror();

    if(pM->get_show_colors())
        set_background_color(QColor(230,239,244));

    int iDisplayMode=pM->get_display_mode();
    int iBlockSize=block_size();
    int iLine=pos().y();
    bool bShowBothSide=pM->get_show_both_side();
/*
    QGraphicsTextItem* ptiTitleTab=new QGraphicsTextItem(" ");
    ptiTitleTab->setPos(pos().x()+iBlockSize*61,iLine);
    add_item(ptiTitleTab);
*/
    MirrorCouderMeasure* mci=static_cast<MirrorCouderMeasure*>(pItem);
    if(iDisplayMode>=1)
    {
        add_line_tab(QObject::tr("Measures:").toStdString(),mci->measures(),pos().x(),iLine,iBlockSize*50);
        iLine+=iBlockSize;

        if(iDisplayMode==2)
        {
            add_line_tab("deltaC:",mci->deltaC(),pos().x(),iLine,iBlockSize*50);
            iLine+=iBlockSize;

            add_line_tab("Lf*1000:",mci->lf1000(),pos().x(),iLine,iBlockSize*50);
            iLine+=iBlockSize;

            add_line_tab("Lf/Ro:",mci->lfro(),pos().x(),iLine,iBlockSize*50);
            iLine+=iBlockSize;

            add_line_tab("-U:",mci->moinsu(),pos().x(),iLine,iBlockSize*50);
            iLine+=iBlockSize*2;
        }
    }

    if(!mci->get_aspect().empty())
    {
        QGraphicsTextItem* ptiAspect=new QGraphicsTextItem(QObject::tr("Aspect: ")+mci->get_aspect().c_str());
        ptiAspect->setPos(pos().x(),iLine);
        add_item(ptiAspect);
        iLine+=iBlockSize;
    }

    QString qsLambdaWave=QObject::tr("LambdaWave=")+QString::number(mci->get_lambda_wave(),'g',3);
    QGraphicsTextItem* ptiLambdaWave=new QGraphicsTextItem(qsLambdaWave);
    ptiLambdaWave->setPos(pos().x(),iLine);
    add_item(ptiLambdaWave);

    QString qsLfro="Lf/Ro=" +QString::number(mci->get_lfro(),'g',2);
    QGraphicsTextItem* ptiLFRO=new QGraphicsTextItem(qsLfro);
    ptiLFRO->setPos(pos().x()+iBlockSize*16,iLine);
    add_item(ptiLFRO);

    QString qsRMS="LambdaRMS="+QString::number(mci->get_lambda_RMS(),'g',3);
    QGraphicsTextItem* ptiRMS=new QGraphicsTextItem(qsRMS);
    ptiRMS->setPos(pos().x()+iBlockSize*31,iLine);
    add_item(ptiRMS);

    QString qsStrehl="Strehl="+QString::number(mci->get_strehl(),'g',2);
    QGraphicsTextItem* ptiStrehl=new QGraphicsTextItem(qsStrehl);
    ptiStrehl->setPos(pos().x()+iBlockSize*46,iLine);
    add_item(ptiStrehl);

    iLine+=iBlockSize;

    //surface boundary
    double dSurfX1=iBlockSize;
    double dSurfX2=iBlockSize*58; //60*iBlockSize=page width
    double dSurfXM=(dSurfX1+dSurfX2)/2.;
    double dSurfY1=iLine+iBlockSize;

    if(iDisplayMode==0)
        iLine+=5*iBlockSize;
    else
        iLine+=8*iBlockSize;


    QGraphicsTextItem* ptiTitleTab2=new QGraphicsTextItem(" ");
    ptiTitleTab2->setPos(pos().x()+iBlockSize*61,iLine-iBlockSize/2);
    add_item(ptiTitleTab2);



    double dSurfY2=iLine;

    double dSurfY12=dSurfY2-dSurfY1;
    double dSurfYM=(dSurfY1+dSurfY2)/2.;

    if(pM->get_show_both_side()==false)
    {
        dSurfXM=dSurfX1;
    }

    const vector<double>& hz=pM->hz();
    double dRadius=pM->diameter()/2.;
    MirrorCouderMeasure* pMeasure=(MirrorCouderMeasure*)pItem;
    const vector<double>&  surf=pMeasure->surface();

    //compute surface MaxHeight
    double dSurfMax=0.;
    for(unsigned int i=0;i<surf.size();i++)
        if(surf[i]>dSurfMax)
            dSurfMax=surf[i];

    if(dSurfMax==0.)
        dSurfMax=1.;

    //plot surface right as a polygon1
    assert(hz.size()==surf.size());
    QPolygonF qpf1;
    for(unsigned int iZ=0;iZ<hz.size();iZ++)
    {
        double dPosX= hz[iZ]/dRadius*(dSurfX2-dSurfXM);
        double dPosY=dSurfY12-dSurfY12*(surf[iZ]/dSurfMax);

        if(iZ==0)
            qpf1.append(QPointF(dSurfXM+dPosX,dSurfY2));

        qpf1.append(QPointF(dSurfXM+dPosX,dSurfY1+dPosY));
    }
    qpf1.append(QPointF(dSurfX2,dSurfY2));

    QGraphicsPolygonItem* polysurf1=new QGraphicsPolygonItem(qpf1);
    polysurf1->setBrush(QColor(127,127,127));
    add_item(polysurf1);

    if(bShowBothSide)
    {
        //plot surface left as a polygon2
        QPolygonF qpf2;
        for(unsigned int iZ=0;iZ<hz.size();iZ++)
        {
            double dPosX= hz[iZ]/dRadius*(dSurfX2-dSurfXM);
            double dPosY=dSurfY12-dSurfY12*(surf[iZ]/dSurfMax);
            if(iZ==0)
                qpf2.append(QPointF(dSurfXM-dPosX,dSurfY2));

            qpf2.append(QPointF(dSurfXM-dPosX,dSurfY1+dPosY));
        }
        qpf2.append(QPointF(dSurfX1,dSurfY2));
        QGraphicsPolygonItem* polysurf2=new QGraphicsPolygonItem(qpf2);
        polysurf2->setBrush(QColor(127,127,127));
        add_item(polysurf2);

    }
    //add vertical zonal mask border
    for(unsigned int iZ=0;iZ<hz.size();iZ++)
    {
        double dPosX= hz[iZ]/dRadius*(dSurfX2-dSurfXM);
        double dBorder1=dSurfXM+dPosX;
        double dBorder2=dSurfXM-dPosX;

        if(bShowBothSide==false)
            dBorder2=dSurfXM;

        // in case of central hole, add a central vertical line and a light gray rect
        if(hz[0]!=0 && (iZ==0) )
        {
            QGraphicsLineItem* li12=new QGraphicsLineItem(dSurfXM,dSurfY1,dSurfXM,dSurfY2);
            add_item(li12);

            QGraphicsLineItem* li21=new QGraphicsLineItem(dBorder2,dSurfY2,dBorder1,dSurfY2);
            li21->setPen(QPen(Qt::DashLine));
            add_item(li21);
        }

        QGraphicsLineItem* li1=new QGraphicsLineItem(dBorder1,dSurfY1,dBorder1,dSurfY2);
        add_item(li1);

        if(bShowBothSide)
        {
            QGraphicsLineItem* li2=new QGraphicsLineItem(dBorder2,dSurfY1,dBorder2,dSurfY2);
            add_item(li2);
        }
        QGraphicsTextItem* ptiSurfaceHeight=new QGraphicsTextItem(QString::number(surf[iZ],'f',0));
        ptiSurfaceHeight->setPos(dBorder1,dSurfYM);
        add_item(ptiSurfaceHeight);
    }
}
////////////////////////////////////////////////////////////////////////////
