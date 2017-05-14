#include "TaskItemWork.h"
#include "Foucault2Defines.h"

#include <QPen>

#include "MirrorWork.h"
#include "Mirror.h"

#include <string>
using namespace std;

TaskItemWork::TaskItemWork(MirrorItem* pItem):TaskItem(pItem)
{
    int iBlockSize=block_size();
    int iLine=(int)pos().y();

    MirrorWork* pMC=static_cast<MirrorWork*>(pItem);
    const Mirror* pM=pItem->mirror();
    int iDisplayMode=pM->get_display_mode();
    if(pM->get_show_colors())
        set_background_color(QColor(247,247,192));

    if(iDisplayMode>=DISPLAY_MODE_NORMAL)
    {
        //Add date and hour
        string s=pMC->when_as_text();
        QGraphicsTextItem* ptiWhen=new QGraphicsTextItem(s.c_str());
        ptiWhen->setPos(pos().x(),iLine);
        add_item(ptiWhen);
        iLine+=iBlockSize;
    }

    int iWT=pMC->work_type();
    QString qsWorkType;

    if(iWT==WORK_TYPE_UNDEFINED)
        qsWorkType=QObject::tr("Work");

    if(iWT==WORK_TYPE_ROUGH_GRINDING)
        qsWorkType=QObject::tr("Rough Grinding");

    if(iWT==WORK_TYPE_FINE_GRINDING)
        qsWorkType=QObject::tr("Fine Grinding");

    if(iWT==WORK_TYPE_POLISHING)
        qsWorkType=QObject::tr("Polishing");

    if(iWT==WORK_TYPE_FIGURING)
        qsWorkType=QObject::tr("Figuring");

    unsigned int iDuration=pMC->duration();
    QString qsDuration="";

    if(iDuration!=0)
    {
        int iHour=iDuration/3600;
        int iMinute=(iDuration %3600)/60;
        int iSecond=(iDuration %60);

        qsDuration=" ("+QObject::tr("duration")+" ";
        if(iHour!=0)
            qsDuration+=QString::number(iHour)+"h";

        if( (iMinute!=0) || ((iSecond!=0)&&(iHour!=0)) )
            qsDuration+=QString::number(iMinute)+"min";

        if( iSecond!=0)
            qsDuration+=QString::number(iSecond)+"s";

        qsDuration+=")";
    }

    QGraphicsTextItem* pti=new QGraphicsTextItem(qsWorkType+qsDuration+": "+pMC->work().c_str());
    pti->setPos(pos().x(),iLine);
    add_item(pti);

    QGraphicsTextItem* ptiTitleTab=new QGraphicsTextItem(" ");
    ptiTitleTab->setPos(pos().x()+iBlockSize*61,iLine);
    add_item(ptiTitleTab);
}
