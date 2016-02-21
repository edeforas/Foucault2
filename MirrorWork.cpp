#include "MirrorWork.h"

MirrorWork::MirrorWork(string sWork):
    _sWork(sWork)
{
    _sType="MirrorWork";
    _uiDuration=0;
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
