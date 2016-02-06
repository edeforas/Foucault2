#include "MirrorComment.h"

MirrorComment::MirrorComment(string sComment):
    _sComment(sComment)
{
    _sType="MirrorComment";
}

string& MirrorComment::comment()
{
    return _sComment;
}
