#ifndef _MirrorComment_
#define _MirrorComment_

#include "MirrorItem.h"
#include <string>
using namespace std;

class MirrorComment : public MirrorItem
{
public:
    MirrorComment(const string& sComment);

    string& comment();

private:
    string _sComment;
};

#endif
