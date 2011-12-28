#include "LexerJava.h"

using namespace Lexer;

Java::Java(const Constructor* constructor) : CPP(constructor) {}

const char *Java::keywords(int set) const
{
    if (set != 1)
        return 0;

    return "abstract assert boolean break byte case catch char class "
           "const continue default do double else extends final finally "
           "float for future generic goto if implements import inner "
           "instanceof int interface long native new null operator outer "
           "package private protected public rest return short static "
           "super switch synchronized this throw throws transient try var "
           "void volatile while";
}