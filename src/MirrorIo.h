// this file is covered by the General Public License version 2 or later
// please see GPL.html for more details and licensing issues

// copyright Etienne de Foras ( the author )
// mailto: etienne.deforas@gmail.com

#ifndef _MirrorIo_
#define _MirrorIo_

#include <string>
using namespace std;

class Mirror;

class MirrorIo
{
public:
    static Mirror* load(string sFile);
    static Mirror* import(string sFile);
    static bool save(Mirror* pmir ,string sFile);
};

#endif

