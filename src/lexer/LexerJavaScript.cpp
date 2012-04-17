#include "LexerJavaScript.h"

#include "LexerFactory.h"

#include <QDebug>

using namespace Lexer;

JavaScript::JavaScript(const Constructor* constructor) : LexerBase(this, constructor)
{
}