#ifndef _TaskItemComment_
#define _TaskItemComment_

#include "TaskItem.h"

class TaskItemComment : public TaskItem
{
public:
    TaskItemComment(MirrorItem* pItem,int iBlockSize);
};

#endif
