#include "TaskItemComment.h"
#include <QPen>

#include "MirrorComment.h"
#include "Mirror.h"

#include <string>
using namespace std;
#include "Foucault2Defines.h"

TaskItemComment::TaskItemComment(MirrorItem* pItem):TaskItem(pItem)
{
    int iBlockSize=block_size();
    int iLine=pos().y();

    MirrorComment* pMC=static_cast<MirrorComment*>(pItem);
    const Mirror* pM=pItem->mirror();
    int iDisplayMode=pM->get_display_mode();

    if(iDisplayMode>=DISPLAY_MODE_NORMAL)
    {
        string s=pMC->when_as_text();
        QGraphicsTextItem* ptiWhen=new QGraphicsTextItem(s.c_str());
        ptiWhen->setPos(pos().x(),iLine);
        add_item(ptiWhen);
        iLine+=iBlockSize;
    }

    QGraphicsTextItem* pti=new QGraphicsTextItem(QString(QObject::tr("Comment: "))+pMC->comment().c_str());
    pti->setPos(pos().x(),iLine);
    add_item(pti);

    QGraphicsTextItem* ptiTitleTab=new QGraphicsTextItem(" ");
    ptiTitleTab->setPos(pos().x()+iBlockSize*61,iLine);
    add_item(ptiTitleTab);

    if(pM->get_show_colors())
        set_background_color(QColor(242,219,222));
}
