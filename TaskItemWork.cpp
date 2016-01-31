#include "TaskItemWork.h"
#include <QPen>

#include "MirrorWork.h"
#include "Mirror.h"

#include <string>
using namespace std;

TaskItemWork::TaskItemWork(MirrorItem* pItem):TaskItem(pItem)
{
    MirrorWork* pMC=(MirrorWork*)pItem;
    Mirror* pM=pItem->mirror();
    if(pM->get_show_colors())
        set_background_color(QColor(247,247,192));

    QGraphicsTextItem* pti=new QGraphicsTextItem(QObject::tr("Work: ")+pMC->work().c_str());
    add_item(pti);
}
