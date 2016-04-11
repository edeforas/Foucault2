#include "TaskItemComment.h"
#include <QPen>

#include "MirrorComment.h"
#include "Mirror.h"

#include <string>
using namespace std;

TaskItemComment::TaskItemComment(MirrorItem* pItem):TaskItem(pItem)
{
    int iBlockSize=block_size();
    int iLine=pos().y();

    MirrorComment* pMC=static_cast<MirrorComment*>(pItem);
    const Mirror* pM=pItem->mirror();
    QGraphicsTextItem* pti=new QGraphicsTextItem(QString(QObject::tr("Comment: "))+pMC->comment().c_str());

    QGraphicsTextItem* ptiTitleTab=new QGraphicsTextItem(" ");
    ptiTitleTab->setPos(pos().x()+iBlockSize*61,iLine);
    add_item(ptiTitleTab);

    if(pM->get_show_colors())
        set_background_color(QColor(242,219,222));
    add_item(pti);
}
