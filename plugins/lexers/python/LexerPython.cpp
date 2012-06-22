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
		return QColor(0, 0, 0);

	case Comment:
	case CommentBlock:
		return SyntaxStandards::commentColor();

	case Number: return SyntaxStandards::numberColor();
	case Keyword: return SyntaxStandards::keywordColor();

	case DoubleQuotedString:
	case SingleQuotedString:
	case TripleDoubleQuotedString:
	case TripleSingleQuotedString:
		return SyntaxStandards::stringColor();

	case Operator: return SyntaxStandards::operatorColor();
	case UnclosedString: return SyntaxStandards::unclosedStringColor();
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