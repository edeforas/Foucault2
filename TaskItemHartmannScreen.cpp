#include "TaskItemHartmannScreen.h"
#include <QPen>

TaskItemHartmannScreen::TaskItemHartmannScreen(MirrorItem* pItem):TaskItem(pItem)
{
    setPen(QPen(QColor(128,128,128)));
    setRect(0,0,100,100);
}
