#include "LexerJavaScript.h"

#include "LexerFactory.h"

#include <QDebug>

using namespace Lexer;

LexerBase* ConstructorJavaScript::construct() const { return Factory::ref().newLexerFromConstructor(this); }
LexerBase* ConstructorJavaScript::construct(const QString& apis) const { return construct(); }
LexerBase* ConstructorJavaScript::_new() const { return new JavaScript(this); }

ConstructorJavaScript::ConstructorJavaScript() : Constructor("Java Script Files") {}

JavaScript::JavaScript(const Constructor* constructor) : LexerBase(this, constructor)
{
}

const bool JavaScript::cStyleBlocks() const { return true; }