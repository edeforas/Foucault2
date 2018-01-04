#include "TaskItemComment.h"
#include "MirrorComment.h"
#include "Mirror.h"
#include "Foucault2Defines.h"

#include <string>
using namespace std;

////////////////////////////////////////////////////////////////////////////
TaskItemComment::TaskItemComment(MirrorItem* pItem,int iBlockSize):TaskItem(pItem,iBlockSize)
{
    int iLine=pos().y();

    MirrorComment* pMC=static_cast<MirrorComment*>(pItem);
    const Mirror* pM=pItem->mirror();
    int iDisplayMode=pM->get_display_mode();

    if(iDisplayMode>=DISPLAY_MODE_NORMAL)
    {
        string s=pMC->when_as_text();
        if(!s.empty())
        {
            QGraphicsTextItem* ptiWhen=new QGraphicsTextItem(s.c_str());
            ptiWhen->setPos(pos().x(),iLine);
            add_item(ptiWhen);
            iLine+=iBlockSize;
        }
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
////////////////////////////////////////////////////////////////////////////
