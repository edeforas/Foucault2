#include "TaskItemWork.h"
#include <QPen>

#include "MirrorWork.h"
#include "Mirror.h"

#include <string>
using namespace std;

TaskItemWork::TaskItemWork(MirrorItem* pItem):TaskItem(pItem)
{
    int iBlockSize=block_size();
    int iLine=pos().y();

    MirrorWork* pMC=static_cast<MirrorWork*>(pItem);
    Mirror* pM=pItem->mirror();
    if(pM->get_show_colors())
        set_background_color(QColor(247,247,192));

    QGraphicsTextItem* ptiTitleTab=new QGraphicsTextItem(" ");
    ptiTitleTab->setPos(pos().x()+iBlockSize*61,iLine);
    add_item(ptiTitleTab);

    QGraphicsTextItem* pti=new QGraphicsTextItem(QObject::tr("Work: ")+pMC->work().c_str());
    add_item(pti);
}
