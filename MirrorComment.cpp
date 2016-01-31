#include "MirrorComment.h"

MirrorComment::MirrorComment(string sComment)
{
    _sType="MirrorComment";
    _sComment=sComment;
}

string& MirrorComment::comment()
{
    return _sComment;
}
