#include "MirrorWork.h"

MirrorWork::MirrorWork(string sWork):
    _sWork(sWork)
{
    _sType="MirrorWork";
}

string& MirrorWork::work()
{
    return _sWork;
}
