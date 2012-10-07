#include "LexerJavaScript.h"

#include "LexerFactory.h"

#include <QDebug>

using namespace Kiss::Lexer;

Base *ConstructorJavaScript::construct() const
{
	return Factory::ref().newLexerFromConstructor(this);
}

Base *ConstructorJavaScript::construct(const QString& apis) const
{
	return construct();
}

Base *ConstructorJavaScript::_new() const
{
	return new JavaScript(this);
}

QStringList ConstructorJavaScript::extensions() const
{
	return QStringList() << "js" << "javascript";
}

ConstructorJavaScript::ConstructorJavaScript()
	: Constructor("Java Script Files")
{
}

JavaScript::JavaScript(const Constructor* constructor)
	: Base(this, constructor)
{
}

const bool JavaScript::cStyleBlocks() const
{
	return true;
}