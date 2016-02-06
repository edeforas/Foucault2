#ifndef _MirrorItem_
#define _MirrorItem_

#include <string>
using namespace std;

class Mirror;

class MirrorItem
{
public:
    MirrorItem();
    virtual ~MirrorItem();

    void set_mirror(Mirror* pMirror);
    Mirror* mirror();
    string type() const;

protected:
    string _sType;
    Mirror* _pParent;
};

#endif
