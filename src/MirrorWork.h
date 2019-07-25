#ifndef _MirrorWork_
#define _MirrorWork_

#include "MirrorItem.h"
#include <string>
using namespace std;

#define WORK_TYPE_UNDEFINED 0
#define WORK_TYPE_ROUGH_GRINDING 1
#define WORK_TYPE_FINE_GRINDING 2
#define WORK_TYPE_POLISHING 3
#define WORK_TYPE_FIGURING 4

class MirrorWork : public MirrorItem
{
public:
    MirrorWork(string sWork);

    string& work();

    void set_duration(unsigned int uiDuration);
    unsigned int duration();

    void set_work_type(int iWorkType);
    int work_type();

private:
    string _sWork;
    int _iWorkType;
    unsigned int _uiDuration;
};

#endif
