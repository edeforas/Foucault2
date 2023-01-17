#include "TaskItemCouderMeasure.h"
#include "Mirror.h"
#include "MirrorCouderMeasure.h"
#include "Foucault2Defines.h"

#include <QPen>
#include <cassert>

////////////////////////////////////////////////////////////////////////////
TaskItemCouderMeasure::TaskItemCouderMeasure(MirrorItem* pItem,int iBlockSize):TaskItem(pItem,iBlockSize)
{
    const Mirror* pM=pItem->mirror();

    if(pM->get_show_colors())
    set_background_color(QColor(236,245,250));

    int iDisplayMode=pM->get_display_mode();
    int iLine=(int)pos().y();
    bool bShowBothSide=pM->get_show_both_side();
    bool bShowLfRo=pM->get_show_lf_ro();
    bool bSmoothCurves=pM->get_smooth_curves();

    MirrorCouderMeasure* mci=static_cast<MirrorCouderMeasure*>(pItem);

    if(iDisplayMode>=DISPLAY_MODE_NORMAL)
    {
        //Add date and hour
        string s=mci->when_as_text();
        if(!s.empty())
        {
            QGraphicsTextItem* ptiWhen=new QGraphicsTextItem(s.c_str());
            ptiWhen->setPos(pos().x(),iLine);
            add_item(ptiWhen);
            iLine+=iBlockSize;
        }
        int iHalfCellMargin=(50/(mci->nb_zones()+1)/2)*iBlockSize;

        add_line_tab(QObject::tr("Hmx:").toStdString(),mci->hmx(),pos().x(),iLine,iBlockSize*50);
        iLine+=iBlockSize;
	
        if(iDisplayMode>=DISPLAY_MODE_FULL)
        {
            add_line_tab("Hm²/R:", mci->hm2r(),pos().x(),iLine,iBlockSize*50);
            iLine+=iBlockSize;

            // compute delta Hm2/R
            vector<double> vd(mci->nb_zones()-1);
            for(unsigned int i=0;i<vd.size();i++)
                vd[i]= mci->hm2r()[i+1] - mci->hm2r()[i];

            add_line_tab("DeltaHm²/R:",vd,pos().x()+iHalfCellMargin,iLine,iBlockSize*50-iHalfCellMargin*2);
            iLine+=iBlockSize;
        }

        add_line_tab(QObject::tr("M (mm):").toStdString(),mci->measures(),pos().x(),iLine,iBlockSize*50);
        iLine+=iBlockSize;

        if(iDisplayMode>=DISPLAY_MODE_DETAIL)
        {
            if(iDisplayMode>=DISPLAY_MODE_FULL)
            {
                // compute delta Measure
                vector<double> vd(mci->nb_zones()-1);
                for(unsigned int i=0;i<vd.size();i++)
                    vd[i]=mci->measures()[i+1]-mci->measures()[i];

                add_line_tab("DeltaMeasure:",vd,pos().x()+iHalfCellMargin,iLine,iBlockSize*50-iHalfCellMargin*2);
                iLine+=iBlockSize;
            }

            add_line_tab("deltaC:",mci->deltaC(),pos().x(),iLine,iBlockSize*50);
            iLine+=iBlockSize;

            add_line_tab("Lf*1000:",mci->lf1000(),pos().x(),iLine,iBlockSize*50);
            iLine+=iBlockSize;

	    //            if(pM->is_parabolic()) // removed P. Crubillé
            {
                add_line_tab("Lf/Ro:",mci->lfro(),pos().x(),iLine,iBlockSize*50);
                iLine+=iBlockSize;
            }

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

    QString qsLfro;
    //    if(pM->is_parabolic())
    qsLfro="Lf/Ro=" +QString::number(mci->get_lfro(),'g',2);
    //    else
    //        qsLfro="Lf/Ro= n/a"; // Modif P. Crubillé

    QGraphicsTextItem* ptiLFRO=new QGraphicsTextItem(qsLfro);
    ptiLFRO->setPos(pos().x()+iBlockSize*16,iLine);
    add_item(ptiLFRO);

    QString qsRMS="LambdaRMS="+QString::number(mci->get_lambda_RMS(),'g',3);
    QGraphicsTextItem* ptiRMS=new QGraphicsTextItem(qsRMS);
    ptiRMS->setPos(pos().x()+iBlockSize*31,iLine);
    add_item(ptiRMS);

    QString qsStrehl;
    if(pM->is_parabolic())
        qsStrehl="Strehl="+QString::number(mci->get_strehl(),'g',2);
    else
        qsStrehl="Strehl= n/a";

    QGraphicsTextItem* ptiStrehl=new QGraphicsTextItem(qsStrehl);
    ptiStrehl->setPos(pos().x()+iBlockSize*46,iLine);
    add_item(ptiStrehl);

    iLine+=2*iBlockSize;
////////////////////////////////////////////////////////////////
//  Draw Surface Graph
////////////////////////////////////////////////////////////////    
    { //surface boundary
    double dSurfX1=iBlockSize;
    double dSurfX2=iBlockSize*58; //60*iBlockSize=page width
    double dSurfXM=(dSurfX1+dSurfX2)/2.;
    double dSurfY1=iLine+iBlockSize;

    if(iDisplayMode==DISPLAY_MODE_COMPACT)
      {
        iLine+=5*iBlockSize;
      }
    else {
        iLine+=8*iBlockSize;
    };

    QGraphicsTextItem* ptiTitleTab2=new QGraphicsTextItem("Surface error");
    ptiTitleTab2->setPos(pos().x()+iBlockSize,iLine +iBlockSize/2);
    add_item(ptiTitleTab2);

    double dSurfY2 = iLine ;	
    double dSurfY12=dSurfY2-dSurfY1;
    double dSurfYM=(dSurfY1+dSurfY2)/2.;

    if(pM->get_show_both_side()==false)
    {
        dSurfXM=dSurfX1;
    }

    const vector<double>& hz= mci->hmz();
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
        pMeasure->compute_surface_smooth(pointsX,pointsY);
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
        qgpiR->setBrush(QColor(210,190,190));
        add_item(qgpiR);

        if(bShowBothSide)
        {
            QGraphicsPathItem* qgpiL=new QGraphicsPathItem(qpathL);
            qgpiL->setBrush(QColor(210,190,190));//setBrush(QColor(192,159,159));
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
        polysurfR->setBrush(QColor(210,190,190));//setBrush(QColor(192,159,159));
        add_item(polysurfR);
        if(bShowBothSide)
        {
            qpolyL.append(QPointF(dSurfXM*2-dSurfX2,dSurfY2));
            QGraphicsPolygonItem* polysurfL=new QGraphicsPolygonItem(qpolyL);
            polysurfL->setBrush(QColor(210,190,190));//setBrush(QColor(192,159,159));
            add_item(polysurfL);
        }
    }

    //add vertical zonal mask border
    QString sSurfUnit=iDisplayMode>=2?" nm":"";
    QString sUnitMm=iDisplayMode>=2?"mm":"";
    float MaxSurfaceHeight = 0; int MaxSurfacedBorder = 0;
    float MinSurfaceHeight = 100000; int MinSurfacedBorder = 0;
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

        if(bShowBothSide)
        {
            QGraphicsLineItem* li2=new QGraphicsLineItem(dBorder2,dSurfY2,dBorder2,dSurfY2-10);
            add_item(li2);
        }
        QGraphicsTextItem* ptiSurfaceHeight=new QGraphicsTextItem(QString::number(surf[iZ],'f',0)+sSurfUnit);
        QGraphicsTextItem* ptiSurfaceOrd=new QGraphicsTextItem(QString::number(hz[iZ],'f',0)+sUnitMm);
	if( (MaxSurfaceHeight < surf[iZ] )&&((iZ>=hz.size()) || (surf[iZ] >= surf[iZ+1])))
	  {
	    QGraphicsLineItem* li1=new QGraphicsLineItem(dBorder1,dSurfY1,dBorder1,dSurfY2);
	    add_item(li1);

	    MaxSurfaceHeight = surf[iZ] ; MaxSurfacedBorder = dBorder1;
	    ptiSurfaceHeight->setPos(MaxSurfacedBorder, dSurfYM -20); // Test P. Crubillé
	    add_item(ptiSurfaceHeight);
	    ptiSurfaceOrd->setPos(MaxSurfacedBorder-20, dSurfY2); // Test P. Crubillé
	    add_item(ptiSurfaceOrd);
	  } else
	  {
	    MaxSurfaceHeight = surf[iZ] ;
	    if( (MinSurfaceHeight > surf[iZ] )&&((iZ>=hz.size()) || (surf[iZ] <= surf[iZ+1])))
	      {
		QGraphicsLineItem* li1=new QGraphicsLineItem(dBorder1,dSurfY1,dBorder1,dSurfY2);
		add_item(li1);

		MinSurfaceHeight = surf[iZ] ; MinSurfacedBorder = dBorder1;
		ptiSurfaceHeight->setPos(MinSurfacedBorder, dSurfYM ); // Test P. Crubillé
		add_item(ptiSurfaceHeight );
		ptiSurfaceOrd->setPos(MinSurfacedBorder-20, dSurfY2); // Test P. Crubillé
		add_item(ptiSurfaceOrd);
	      }  else
	      {
		QGraphicsLineItem* li1=new QGraphicsLineItem(dBorder1,dSurfY2,dBorder1,dSurfY2-10);
		add_item(li1);

		MinSurfaceHeight = surf[iZ] ;
	      }
	  }
    }
      //////////////// add obstruction mask polygon //
    {
      QPolygonF qpolyObsR, qpolyObsL; 
    double dPosX= hz[0] / dRadius*( dSurfX2-dSurfXM );
    double dPosY=dSurfY12- dSurfY12;
    qpolyObsR.append(QPointF(dSurfXM+dPosX, dSurfY2));
    qpolyObsR.append(QPointF(dSurfXM+dPosX, dSurfY1));
    if(bShowBothSide){
      qpolyObsL.append(QPointF(dSurfXM-dPosX, dSurfY2));
      qpolyObsL.append(QPointF(dSurfXM-dPosX, dSurfY1));
    }
    dPosX= pM->obstruction_size()/(2*dRadius)*(dSurfX2-dSurfXM);
    dPosY= dSurfY12-(dSurfY12);
    qpolyObsR.append(QPointF(dSurfXM+dPosX, dSurfY1));
    qpolyObsR.append(QPointF(dSurfXM+dPosX, dSurfY2));
    if(bShowBothSide){
      qpolyObsL.append(QPointF(dSurfXM-dPosX, dSurfY1));
      qpolyObsL.append(QPointF(dSurfXM-dPosX, dSurfY2));
    }
    QGraphicsPolygonItem* polysurfObsR=new QGraphicsPolygonItem(qpolyObsR);
    polysurfObsR->setBrush(QColor(235,235,235));
    add_item(polysurfObsR);
    if(bShowBothSide)
      {
	QGraphicsPolygonItem* polysurfObsL=new QGraphicsPolygonItem(qpolyObsL);
	polysurfObsL->setBrush(QColor(240,240,240));//setBrush(QColor(159,192,159));
	add_item(polysurfObsL);
      }

      //////////////// add edge mask polygon //
    {
    QPolygonF qpolyEdgeR, qpolyEdgeL;
    double dPosX= pM->diameter() / (2*dRadius)*( dSurfX2-dSurfXM );
    double dPosY=dSurfY12- dSurfY12;
    qpolyEdgeR.append(QPointF(dSurfXM+dPosX, dSurfY2));
    qpolyEdgeR.append(QPointF(dSurfXM+dPosX, dSurfY1));
    if(bShowBothSide){
      qpolyEdgeL.append(QPointF(dSurfXM-dPosX, dSurfY2));
      qpolyEdgeL.append(QPointF(dSurfXM-dPosX, dSurfY1));
    }
    dPosX= (pM->diameter()/2 - pM->edge_mask_width())/dRadius *(dSurfX2-dSurfXM);
    dPosY= dSurfY12-(dSurfY12);
    qpolyEdgeR.append(QPointF(dSurfXM+dPosX, dSurfY1));
    qpolyEdgeR.append(QPointF(dSurfXM+dPosX, dSurfY2));
    if(bShowBothSide){
      qpolyEdgeL.append(QPointF(dSurfXM-dPosX, dSurfY1));
      qpolyEdgeL.append(QPointF(dSurfXM-dPosX, dSurfY2));
    }
    QGraphicsPolygonItem* polysurfEdgeR=new QGraphicsPolygonItem(qpolyEdgeR);
    polysurfEdgeR->setBrush(QColor(235,235,235));
    add_item(polysurfEdgeR);
    if(bShowBothSide)
      {
	QGraphicsPolygonItem* polysurfEdgeL=new QGraphicsPolygonItem(qpolyEdgeL);
	polysurfEdgeL->setBrush(QColor(240,240,240));//setBrush(QColor(159,192,159));
	add_item(polysurfEdgeL);
      }

    }
    }
    }
    
////////////////////////////////////////////////////////////////
//  Draw Lf/Ro Graph
////////////////////////////////////////////////////////////////    
    iLine+=iBlockSize;
    if(bShowLfRo)
    {
      //surface boundary
    double dSurfX1=iBlockSize;
    double dSurfX2=iBlockSize*58; //60*iBlockSize=page width
    double dSurfXM=(dSurfX1+dSurfX2)/2.;
    double dSurfY1=iLine+iBlockSize;
    double dSurfY2 ;
    if(iDisplayMode==DISPLAY_MODE_COMPACT)
      {
        iLine+=6*iBlockSize;
	dSurfY2= dSurfY1 + 3*iBlockSize;
      }
    else {
        iLine+=10*iBlockSize;
	dSurfY2= dSurfY1 + 5*iBlockSize;
    };


    QGraphicsTextItem* ptiTitleTab2=new QGraphicsTextItem("Transverse error");
    //    ptiTitleTab2->setPos(pos().x()+iBlockSize*61,iLine-iBlockSize/2);
    ptiTitleTab2->setPos(pos().x()+iBlockSize,iLine+iBlockSize );
    add_item(ptiTitleTab2);

    double dSurfY12=dSurfY2-dSurfY1;
    double dSurfYM=(dSurfY1+dSurfY2)/2.;

    if(pM->get_show_both_side()==false)
    {
        dSurfXM=dSurfX1;
    }

    const vector<double>& hz= mci->hmz();
    double dRadius=pM->diameter()/2.;
    MirrorCouderMeasure* pMeasure=static_cast<MirrorCouderMeasure*>(pItem);
    const vector<double>&  lfro=pMeasure->lfro();
    assert((hz.size()-1)==lfro.size());

    // Here are the min and max index including obstruction and edge mask
    unsigned obs_i = 0;
    unsigned edge_mask_i = (unsigned int)hz.size()-1;
    for(unsigned int i=1;i<(hz.size()-1);i++)
      {
	if( hz[ i-1 ] < pM->obstruction_size()/2) obs_i = i;
      }
    for(unsigned int i=(unsigned int)hz.size()-1; i > 2; i--)
      {
	if( hz[ i ] > ( pM->diameter()/2 - pM->edge_mask_width() ))
	    edge_mask_i = i-1;
      }
    unsigned int lfrosz = (unsigned int)lfro.size() ;
    printf("index :: obs = %d - edge = %d - lfro.size = %d \n" , obs_i, edge_mask_i, lfrosz );

    
    //compute surface Min and MaxHeight
    double dLfroMax=0.; double dLfroMin=0.;
    for(unsigned int i= obs_i;i<edge_mask_i;i++)
      {
        if(lfro[i]>dLfroMax) dLfroMax=lfro[i];
        if(lfro[i]<dLfroMin) dLfroMin=lfro[i];
      }
    if(dLfroMax<=1.) dLfroMax=1.;
    if(dLfroMin>=-1.) dLfroMin=-1.;
    if(dLfroMax<=-dLfroMin) dLfroMax=-dLfroMin;

    /*    if(bSmoothCurves)
    {
        vector<double> pointsX,pointsY;
        pMeasure->compute_surface_smooth(pointsX,pointsY);
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
        qgpiR->setBrush(QColor(159,192,159));
        add_item(qgpiR);

        if(bShowBothSide)
        {
            QGraphicsPathItem* qgpiL=new QGraphicsPathItem(qpathL);
            qgpiL->setBrush(QColor(159,192,159));
            add_item(qgpiL);
        }
    }
    else */
    {
        //plot right lfro as a polygon1 (set of lines)
      assert((hz.size()-1)==lfro.size());
      QPolygonF qpolyR,qpolyL;
      double dPosX= hz[0] / dRadius*( dSurfX2-dSurfXM );
      double dPosY=dSurfY12-(dSurfY12)*(lfro[0]/(dLfroMax));
      qpolyR.append(QPointF(dSurfXM+dPosX,dSurfY2));
      if(bShowBothSide)
	qpolyL.append(QPointF(dSurfXM-dPosX,dSurfY2));
      //      for(unsigned int iZ=0;iZ<lfro.size();iZ++)
      for(unsigned int iZ= obs_i;iZ<edge_mask_i;iZ++)
        {
	  dPosX= hz[iZ+1]/dRadius*(dSurfX2-dSurfXM);
	  dPosY=dSurfY12-(dSurfY12)*(lfro[iZ]/(dLfroMax));
	  
	  qpolyR.append(QPointF(dSurfXM+dPosX,dSurfY1+dPosY));
	  if(bShowBothSide)
	    qpolyL.append(QPointF(dSurfXM-dPosX,dSurfY1+dPosY));
        }
      
      qpolyR.append(QPointF(dSurfX2,dSurfY2));
      QGraphicsPolygonItem* polysurfR=new QGraphicsPolygonItem(qpolyR);
      polysurfR->setBrush(QColor(185,210,185));
      add_item(polysurfR);
        if(bShowBothSide)
        {
            qpolyL.append(QPointF(dSurfXM*2-dSurfX2,dSurfY2));
            QGraphicsPolygonItem* polysurfL=new QGraphicsPolygonItem(qpolyL);
            polysurfL->setBrush(QColor(185,210,185));//setBrush(QColor(159,192,159));
            add_item(polysurfL);
        }
    }

      //////////////// add obstruction mask polygon //
    {
      QPolygonF qpolyObsR, qpolyObsL; 
    double dPosX= hz[0] / dRadius*( dSurfX2-dSurfXM );
    double dPosY=dSurfY12- dSurfY12;
    qpolyObsR.append(QPointF(dSurfXM+dPosX, dSurfY2+dSurfY12));
    qpolyObsR.append(QPointF(dSurfXM+dPosX, dSurfY1));
    if(bShowBothSide){
      qpolyObsL.append(QPointF(dSurfXM-dPosX, dSurfY2+dSurfY12));
      qpolyObsL.append(QPointF(dSurfXM-dPosX, dSurfY1));
    }
    dPosX= pM->obstruction_size()/(2*dRadius)*(dSurfX2-dSurfXM);
    dPosY= dSurfY12-(dSurfY12);
    qpolyObsR.append(QPointF(dSurfXM+dPosX, dSurfY1));
    qpolyObsR.append(QPointF(dSurfXM+dPosX, dSurfY2+dSurfY12));
    if(bShowBothSide){
      qpolyObsL.append(QPointF(dSurfXM-dPosX, dSurfY1));
      qpolyObsL.append(QPointF(dSurfXM-dPosX, dSurfY2+dSurfY12));
    }
    QGraphicsPolygonItem* polysurfObsR=new QGraphicsPolygonItem(qpolyObsR);
    polysurfObsR->setBrush(QColor(235,235,235));
    add_item(polysurfObsR);
    if(bShowBothSide)
      {
	QGraphicsPolygonItem* polysurfObsL=new QGraphicsPolygonItem(qpolyObsL);
	polysurfObsL->setBrush(QColor(240,240,240));//setBrush(QColor(159,192,159));
	add_item(polysurfObsL);
      }

    }
      //////////////// add edge mask polygon //
    {
    QPolygonF qpolyEdgeR, qpolyEdgeL;
    double dPosX= pM->diameter() / (2*dRadius)*( dSurfX2-dSurfXM );
    double dPosY=dSurfY12- dSurfY12;
    qpolyEdgeR.append(QPointF(dSurfXM+dPosX, dSurfY2+dSurfY12));
    qpolyEdgeR.append(QPointF(dSurfXM+dPosX, dSurfY1));
    if(bShowBothSide){
      qpolyEdgeL.append(QPointF(dSurfXM-dPosX, dSurfY2+dSurfY12));
      qpolyEdgeL.append(QPointF(dSurfXM-dPosX, dSurfY1));
    }
    dPosX= ( pM->diameter()/2 - pM->edge_mask_width())/dRadius *(dSurfX2-dSurfXM);
    dPosY= dSurfY12-(dSurfY12);
    qpolyEdgeR.append(QPointF(dSurfXM+dPosX, dSurfY1));
    qpolyEdgeR.append(QPointF(dSurfXM+dPosX, dSurfY2+dSurfY12));
    if(bShowBothSide){
      qpolyEdgeL.append(QPointF(dSurfXM-dPosX, dSurfY1));
      qpolyEdgeL.append(QPointF(dSurfXM-dPosX, dSurfY2+dSurfY12));
    }
    QGraphicsPolygonItem* polysurfEdgeR=new QGraphicsPolygonItem(qpolyEdgeR);
    polysurfEdgeR->setBrush(QColor(235,235,235));
    add_item(polysurfEdgeR);
    if(bShowBothSide)
      {
	QGraphicsPolygonItem* polysurfEdgeL=new QGraphicsPolygonItem(qpolyEdgeL);
	polysurfEdgeL->setBrush(QColor(240,240,240));//setBrush(QColor(159,192,159));
	add_item(polysurfEdgeL);
      }

    }
    /////////////////  add vertical zonal mask border
    QString sSurfUnit=iDisplayMode>=2?" ":"";
    QString sUnitMm=iDisplayMode>=2?"mm":"";
    float MaxSurfaceHeight = 0; int MaxSurfacedBorder = 0;
    float MinSurfaceHeight = 100000; int MinSurfacedBorder = 0;
  for(unsigned int iZ=0;iZ<lfro.size();iZ++)
    {
      double dPosX= hz[iZ+1]/dRadius*(dSurfX2-dSurfXM); // +1 as 0 or hole radius is inserted in front of hz
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

        if(bShowBothSide)
        {
            QGraphicsLineItem* li2=new QGraphicsLineItem(dBorder2,dSurfY2,dBorder2,dSurfY2-10);
            add_item(li2);
        }
        QGraphicsTextItem* ptiSurfaceHeight=new QGraphicsTextItem(QString::number(lfro[iZ],'f',2)+sSurfUnit);
        QGraphicsTextItem* ptiSurfaceOrd=new QGraphicsTextItem(QString::number(hz[iZ+1],'f',0)+sUnitMm);
	if( (MaxSurfaceHeight < lfro[iZ] )&&((iZ>=(lfro.size()-1)) || (lfro[iZ] >= lfro[iZ+1])))
	  {
	    MaxSurfaceHeight = lfro[iZ] ; MaxSurfacedBorder = dBorder1;
	    if(MaxSurfaceHeight > 0){
	      QGraphicsLineItem* li1=new QGraphicsLineItem(dBorder1,dSurfY1,dBorder1,dSurfY2);
	      add_item(li1);

	      ptiSurfaceHeight->setPos(MaxSurfacedBorder, dSurfYM); 
	      add_item(ptiSurfaceHeight);
	      ptiSurfaceOrd->setPos(MaxSurfacedBorder-20, dSurfY2); 
	      add_item(ptiSurfaceOrd);
	    }
	  } else
	  {
	    MaxSurfaceHeight = lfro[iZ] ;
	    if( (MinSurfaceHeight > lfro[iZ] )&&((iZ>=(lfro.size()-1)) || (lfro[iZ] < lfro[iZ+1])))
	      {
		MinSurfaceHeight = lfro[iZ] ; MinSurfacedBorder = dBorder1;
		if(MinSurfaceHeight < 0){
		  QGraphicsLineItem* li1=new QGraphicsLineItem(dBorder1,dSurfY2+dSurfY12,dBorder1,dSurfY2);
		  add_item(li1);

		  ptiSurfaceHeight->setPos(MinSurfacedBorder, dSurfY12/2 +dSurfY2); // Test P. Crubillé
		  add_item(ptiSurfaceHeight );
		  ptiSurfaceOrd->setPos(MinSurfacedBorder-20, dSurfY2); // Test P. Crubillé
		  add_item(ptiSurfaceOrd);
		}
	      }  else
	      {
		QGraphicsLineItem* li1=new QGraphicsLineItem(dBorder1,dSurfY2,dBorder1,dSurfY2-10);
		add_item(li1);

		MinSurfaceHeight = lfro[iZ] ;
	      }
	  }
    }
    iLine+=iBlockSize;
    }
}
////////////////////////////////////////////////////////////////////////////
