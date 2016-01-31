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

private:
    string _sWork;
};

#endif
