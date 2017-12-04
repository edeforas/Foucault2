#ifndef _TaskItemWork_
#define _TaskItemWork_

#include "TaskItem.h"

class TaskItemWork : public TaskItem
{
public:
    TaskItemWork(MirrorItem* pItem,int iBlockSize);
};

#endif
