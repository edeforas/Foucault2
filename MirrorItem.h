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
    const Mirror* mirror() const;
    string type() const;

    void set_when(long lWhen);
    long when() const;
    string when_as_text() const;

protected:
    string _sType;
    Mirror* _pParent;
    long _lWhen;
};

#endif
