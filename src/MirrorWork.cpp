#include "MirrorWork.h"

MirrorWork::MirrorWork(string sWork):
    _sWork(sWork)
{
    _sType="MirrorWork";
    _uiDuration=0;

    _iWorkType=WORK_TYPE_UNDEFINED;
}

string& MirrorWork::work()
{
    return _sWork;
}

void MirrorWork::set_duration(unsigned int uiDuration)
{
    _uiDuration=uiDuration;
}

unsigned int MirrorWork::duration()
{
    return _uiDuration;
}

void MirrorWork::set_work_type(int iWorkType)
{
    _iWorkType=iWorkType;
}

int MirrorWork::work_type()
{
    return _iWorkType;
}
