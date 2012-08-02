#include "LexerPython.h"
#include "LexerFactory.h"

#include "SyntaxStandards.h"

#include <QtPlugin>

using namespace Lexer;

LexerBase* ConstructorPython::construct() const { return Factory::ref().newLexerFromConstructor(this); }
LexerBase* ConstructorPython::construct(const QString& apis) const { return construct(); }
LexerBase* ConstructorPython::_new() const { return new Python(this); }
QStringList ConstructorPython::extensions() const { return QStringList() << "py" << "python"; }

ConstructorPython::ConstructorPython() : Constructor("Python Files") {}

Python::Python(const Constructor* constructor) : LexerBase(this, constructor)
{
}

QColor Python::defaultColor(int style) const
{
    switch (style)
    {
	case Default: 
	case ClassName:
	case FunctionMethodName:
	case Identifier:
	case HighlightedIdentifier:
		return Lexer::Settings::ref().getSetting(DEFAULT, SyntaxStandards::defaultColor());

	case Comment:
	case CommentBlock:
		return Lexer::Settings::ref().getSetting(COMMENT, SyntaxStandards::commentColor());

	case Number: return Lexer::Settings::ref().getSetting(NUMBER, SyntaxStandards::numberColor());
	case Keyword: return Lexer::Settings::ref().getSetting(KEYWORD, SyntaxStandards::keywordColor());

	case DoubleQuotedString:
	case SingleQuotedString:
	case TripleDoubleQuotedString:
	case TripleSingleQuotedString:
		return Lexer::Settings::ref().getSetting(STRING, SyntaxStandards::stringColor());

	case Operator: return Lexer::Settings::ref().getSetting(OPERATOR, SyntaxStandards::operatorColor());
	case UnclosedString: return Lexer::Settings::ref().getSetting(UNCLOSED_STRING, SyntaxStandards::unclosedStringColor());
    }

    return QsciLexerPython::defaultColor(style);
}

QFont Python::font(int style) const
{
	QFont f = QsciLexer::font(style);

	switch (style)
	{
	case Keyword:
	case Operator:
		f.setBold(true);
		break;
	}
	
	return f;
}

QFont Python::defaultFont(int style) const
{
	QFont f = QsciLexer::defaultFont(style);

	switch (style)
	{
	case Keyword:
	case Operator:
		f.setBold(true);
		break;
	}


	return f;
}

const bool Python::cStyleBlocks() const { return false; }

Q_EXPORT_PLUGIN2(python_lexer, Lexer::ConstructorPython); 
