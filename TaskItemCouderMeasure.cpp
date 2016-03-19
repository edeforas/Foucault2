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
    bool bSmoothCurves=pM->get_smooth_curves();

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
        //plot right surface as a smoothcurve
        QPainterPath qpathR,qpathL;
        for(unsigned int iZ=1;iZ<hz.size()-1;iZ++)
        {
            double X1= hz[iZ-1]/dRadius*(dSurfX2-dSurfXM)+dSurfXM;
            double X2= hz[iZ]/dRadius*(dSurfX2-dSurfXM)+dSurfXM;
            double X3= hz[iZ+1]/dRadius*(dSurfX2-dSurfXM)+dSurfXM;

            double X12=(X1+X2)/2.;
            double X23=(X2+X3)/2.;

            double Y1=dSurfY2-dSurfY12*(surf[iZ-1]/dSurfMax);
            double Y2=dSurfY2-dSurfY12*(surf[iZ]/dSurfMax);
            double Y3=dSurfY2-dSurfY12*(surf[iZ+1]/dSurfMax);

            double Y12=(Y1+Y2)/2.;
            double Y23=(Y2+Y3)/2.;

            if(iZ==1)
            {
                //fit a parabolic y=Ax*x+B, that link and slope to X12,Y12
                assert(X2!=X1); assert(X12!=0.);
                double dSlope12=(Y2-Y1)/(X2-X1);
                double dA=dSlope12/2./(X12-dSurfXM);
                double dB=Y12-dA*(X12-dSurfXM)*(X12-dSurfXM);

                //find end point and control point
                double Y1Parab=dA*(X1-dSurfXM)*(X1-dSurfXM)+dB;
                double XM=(X1+X12)/2.;
                double YM=dA*(XM-dSurfXM)*(XM-dSurfXM)+dB;
                double XC=2*XM-(X1+X12)/2.;
                double YC=2*YM-(Y1Parab+Y12)/2.;

                //draw
                qpathR.moveTo(X1,dSurfY2);
                qpathR.lineTo(X1,Y1Parab);
                qpathR.quadTo(XC,YC,X12,Y12);

                if(bShowBothSide)
                {
                    qpathL.moveTo(2.*dSurfXM-X1,dSurfY2);
                    qpathL.lineTo(2.*dSurfXM-X1,Y1Parab);
                    qpathL.quadTo(2.*dSurfXM-XC,YC,2.*dSurfXM-X12,Y12);
                }
            }

            qpathR.quadTo(X2,Y2,X23,Y23);
            if(bShowBothSide)
            {
                qpathL.quadTo(2.*dSurfXM-X2,Y2,2.*dSurfXM-X23,Y23);
            }
            if(iZ==hz.size()-2)
            {
                qpathR.lineTo(X3,Y3);
                qpathR.lineTo(X3,dSurfY2);

                if(bShowBothSide)
                {
                    qpathL.lineTo(2.*dSurfXM-X3,Y3);
                    qpathL.lineTo(2.*dSurfXM-X3,dSurfY2);
                }
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
        //plot right surface as a polygon1
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
