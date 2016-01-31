#include "MirrorWork.h"

MirrorWork::MirrorWork(string sWork)
{
    _sType="MirrorWork";
    _sWork=sWork;
}

string& MirrorWork::work()
{
    return _sWork;
}
