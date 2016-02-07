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

    void set_when(unsigned int uiWhen);
    unsigned int when();

protected:
    string _sType;
    Mirror* _pParent;
    unsigned int _uiWhen;
};

#endif
