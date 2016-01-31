#include "TaskItemCouderScreen.h"
#include <QPen>

TaskItemCouderScreen::TaskItemCouderScreen(MirrorItem* pItem):TaskItem(pItem)
{
    setPen(QPen(QColor(128,128,128)));
    setRect(0,0,100,100);
}
