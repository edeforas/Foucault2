#ifndef _MirrorWork_
#define _MirrorWork_

#include "MirrorItem.h"
#include <string>
using namespace std;

class MirrorWork : public MirrorItem
{
public:
    MirrorWork(string sWork);

    string& work();

    void set_duration(unsigned int uiDuration);
    unsigned int duration();

private:
    string _sWork;
    unsigned int _uiDuration;
};

#endif
