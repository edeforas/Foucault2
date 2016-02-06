#include "TaskItemComment.h"
#include <QPen>

#include "MirrorComment.h"
#include "Mirror.h"

#include <string>
using namespace std;

TaskItemComment::TaskItemComment(MirrorItem* pItem):TaskItem(pItem)
{
    MirrorComment* pMC=static_cast<MirrorComment*>(pItem);
    Mirror* pM=pItem->mirror();
    QGraphicsTextItem* pti=new QGraphicsTextItem(QString(QObject::tr("Comment: "))+pMC->comment().c_str());

    if(pM->get_show_colors())
        set_background_color(QColor(242,219,222));
    add_item(pti);
}
