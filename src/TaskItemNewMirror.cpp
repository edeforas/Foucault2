#include "TaskItemNewMirror.h"
#include "MirrorItem.h"
#include "Mirror.h"
#include "Foucault2Defines.h"

#include <string>
using namespace std;

///////////////////////////////////////////////////////////////////////
TaskItemNewMirror::TaskItemNewMirror(MirrorItem* pItem,int iBlockSize):TaskItem(pItem,iBlockSize)
{
    const Mirror* pM=pItem->mirror();
    int iLine=pos().y();
    int iDisplayMode=pM->get_display_mode();
    if(pM->get_show_colors())
        set_background_color(QColor(166,184,221));

    QGraphicsTextItem* ptiTitle=new QGraphicsTextItem(pM->name().c_str());
    ptiTitle->setScale(2);
    ptiTitle->setPos(pos().x(),iLine);
    add_item(ptiTitle);
    iLine+=iBlockSize*3;

    QGraphicsTextItem* ptiTitleTab=new QGraphicsTextItem(" ");
    ptiTitleTab->setPos(pos().x()+iBlockSize*61,iLine);
    add_item(ptiTitleTab);

    QGraphicsTextItem* ptiDiameter=new QGraphicsTextItem(QObject::tr("Diameter: ")+QString::number(pM->diameter())+QString(" mm"));
    ptiDiameter->setPos(pos().x(),iLine);
    add_item(ptiDiameter);

    QGraphicsTextItem* ptiHoleDiameter=new QGraphicsTextItem(QObject::tr("Hole Diameter: ")+QString::number(pM->hole_diameter())+ QString(" mm"));
    ptiHoleDiameter->setPos(pos().x()+iBlockSize*16,iLine);
    add_item(ptiHoleDiameter);
    iLine+=iBlockSize;

    QGraphicsTextItem* ptiFocal=new QGraphicsTextItem(QObject::tr("Focal Length: ")+QString::number(pM->focal())+QString(" mm"));
    ptiFocal->setPos(pos().x(),iLine);
    add_item(ptiFocal);

    QGraphicsTextItem* ptiConic=new QGraphicsTextItem(QObject::tr("Conical: ")+QString::number(pM->conical()));
    ptiConic->setPos(pos().x()+iBlockSize*16,pos().y()+iLine);
    add_item(ptiConic);
    iLine+=iBlockSize;

    QGraphicsTextItem* ptinbz=new QGraphicsTextItem(QString("NbZones: ")+QString::number(pM->nb_zones()));
    ptinbz->setPos(pos().x(),iLine);
    add_item(ptinbz);

    QGraphicsTextItem* ptiLight=new QGraphicsTextItem(pM->is_slit_moving()?QObject::tr("LigthSlit: Moving"):QObject::tr("LightSlit: Still"));
    ptiLight->setPos(pos().x()+iBlockSize*16,iLine);
    add_item(ptiLight);
    iLine+=iBlockSize;

    if(iDisplayMode>=DISPLAY_MODE_DETAIL)
        iLine+=iBlockSize;

    if(iDisplayMode>=DISPLAY_MODE_NORMAL)
    {
        vector<double> vdZone;
        for(unsigned int i=0;i<pM->hx().size();i++)
            vdZone.push_back(i+1);
        add_line_tab("Zone:",vdZone,pos().x(),iLine,iBlockSize*50,true,true);
        iLine+=iBlockSize;
    }

    add_line_tab("Hx:",pM->hx(),pos().x(),iLine,iBlockSize*50);
    iLine+=iBlockSize;

    if(iDisplayMode>=DISPLAY_MODE_DETAIL)
    {
        add_line_tab("Hm²/R:",pM->hm2r(),pos().x(),iLine,iBlockSize*50);
        iLine+=iBlockSize;

        add_line_tab("Hm/4F:",pM->hm4f(),pos().x(),iLine,iBlockSize*50);
        iLine+=iBlockSize;

        add_line_tab("RelSurface:",pM->relative_surface(),pos().x(),iLine,iBlockSize*50);
    }
}
///////////////////////////////////////////////////////////////////////
