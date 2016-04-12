#include "TaskItemCouderMeasure.h"
#include <QPen>
#include "Mirror.h"
#include "MirrorCouderMeasure.h"
#include <cassert>

TaskItemCouderMeasure::TaskItemCouderMeasure(MirrorItem* pItem):TaskItem(pItem)
{
    const Mirror* pM=pItem->mirror();

    if(pM->get_show_colors())
        set_background_color(QColor(230,239,244));

    int iDisplayMode=pM->get_display_mode();
    int iBlockSize=block_size();
    int iLine=pos().y();
    bool bShowBothSide=pM->get_show_both_side();
    bool bSmoothCurves=pM->get_smooth_curves();

    MirrorCouderMeasure* mci=static_cast<MirrorCouderMeasure*>(pItem);
    if(iDisplayMode>=1)
    {
        add_line_tab(QObject::tr("Measures (mm):").toStdString(),mci->measures(),pos().x(),iLine,iBlockSize*50);
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
    MirrorCouderMeasure* pMeasure=static_cast<MirrorCouderMeasure*>(pItem);
    const vector<double>&  surf=pMeasure->surface();
    assert(hz.size()==surf.size());

    //compute surface MaxHeight
    double dSurfMax=0.;
    for(unsigned int i=0;i<surf.size();i++)
        if(surf[i]>dSurfMax)
            dSurfMax=surf[i];

    if(dSurfMax==0.)
        dSurfMax=1.;

    if(bSmoothCurves)
    {
        vector<double> pointsX,pointsY;
        pMeasure->get_surface_smooth(pointsX,pointsY);
        assert(pointsX.size()==hz.size()+hz.size()+1);
        assert(pointsY.size()==hz.size()+hz.size()+1);

        //plot right surface as a smoothcurve
        QPainterPath qpathR,qpathL;
        qpathR.moveTo( pointsX[0]/dRadius*(dSurfX2-dSurfXM)+dSurfXM,dSurfY2);

        qpathR.lineTo(
                pointsX[0]/dRadius*(dSurfX2-dSurfXM)+dSurfXM,
                dSurfY2-dSurfY12*(pointsY[0]/dSurfMax)
                );

        if(bShowBothSide)
        {
            qpathL.moveTo(pointsX[0]/dRadius*(-dSurfX2+dSurfXM)+dSurfXM,dSurfY2);

            qpathL.lineTo(
                    pointsX[0]/dRadius*(-dSurfX2+dSurfXM)+dSurfXM,
                    dSurfY2-dSurfY12*(pointsY[0]/dSurfMax)
                         );
        }

        for(unsigned int iZ=1;iZ<hz.size()+1;iZ++)
        {
            qpathR.quadTo(
                    pointsX[iZ*2-1]/dRadius*(dSurfX2-dSurfXM)+dSurfXM,
                    dSurfY2-dSurfY12*(pointsY[iZ*2-1]/dSurfMax),
                    pointsX[iZ*2]/dRadius*(dSurfX2-dSurfXM)+dSurfXM,
                    dSurfY2-dSurfY12*(pointsY[iZ*2]/dSurfMax)
                    );

            if(bShowBothSide)
            {
                qpathL.quadTo(
                        pointsX[iZ*2-1]/dRadius*(-dSurfX2+dSurfXM)+dSurfXM,
                        dSurfY2-dSurfY12*(pointsY[iZ*2-1]/dSurfMax),
                        pointsX[iZ*2]/dRadius*(-dSurfX2+dSurfXM)+dSurfXM,
                        dSurfY2-dSurfY12*(pointsY[iZ*2]/dSurfMax)
                        );
            }

            if(iZ==hz.size())
            {
                qpathR.lineTo(pointsX[iZ*2]/dRadius*(dSurfX2-dSurfXM)+dSurfXM,dSurfY2);
                if(bShowBothSide)
                    qpathL.lineTo(pointsX[iZ*2]/dRadius*(-dSurfX2+dSurfXM)+dSurfXM,dSurfY2);
            }
        }

        QGraphicsPathItem* qgpiR=new QGraphicsPathItem(qpathR);
        qgpiR->setBrush(QColor(127,127,127));
        add_item(qgpiR);

        if(bShowBothSide)
        {
            QGraphicsPathItem* qgpiL=new QGraphicsPathItem(qpathL);
            qgpiL->setBrush(QColor(127,127,127));
            add_item(qgpiL);
        }
    }
    else
    {
        //plot right surface as a polygon1 (set of lines)
        assert(hz.size()==surf.size());
        QPolygonF qpolyR,qpolyL;
        for(unsigned int iZ=0;iZ<hz.size();iZ++)
        {
            double dPosX= hz[iZ]/dRadius*(dSurfX2-dSurfXM);
            double dPosY=dSurfY12-dSurfY12*(surf[iZ]/dSurfMax);

            if(iZ==0)
            {
                qpolyR.append(QPointF(dSurfXM+dPosX,dSurfY2));
                if(bShowBothSide)
                    qpolyL.append(QPointF(dSurfXM-dPosX,dSurfY2));
            }
            qpolyR.append(QPointF(dSurfXM+dPosX,dSurfY1+dPosY));
            if(bShowBothSide)
                qpolyL.append(QPointF(dSurfXM-dPosX,dSurfY1+dPosY));
        }

        qpolyR.append(QPointF(dSurfX2,dSurfY2));
        QGraphicsPolygonItem* polysurfR=new QGraphicsPolygonItem(qpolyR);
        polysurfR->setBrush(QColor(127,127,127));
        add_item(polysurfR);
        if(bShowBothSide)
        {
            qpolyL.append(QPointF(dSurfXM*2-dSurfX2,dSurfY2));
            QGraphicsPolygonItem* polysurfL=new QGraphicsPolygonItem(qpolyL);
            polysurfL->setBrush(QColor(127,127,127));
            add_item(polysurfL);
        }
    }

    //add vertical zonal mask border
    QString sSurfUnit=iDisplayMode>=2?" nm":"";
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
        QGraphicsTextItem* ptiSurfaceHeight=new QGraphicsTextItem(QString::number(surf[iZ],'f',0)+sSurfUnit);
        ptiSurfaceHeight->setPos(dBorder1,dSurfYM);
        add_item(ptiSurfaceHeight);
    }
}
////////////////////////////////////////////////////////////////////////////
