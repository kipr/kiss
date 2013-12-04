#include "lexer_python.hpp"
#include "lexer_factory.hpp"

#include "syntax_standards.hpp"

#include <QtPlugin>

using namespace kiss::lexer;

Base *ConstructorPython::construct() const
{
	return Factory::ref().newLexerFromConstructor(this);
}

Base *ConstructorPython::construct(const QString& apis) const
{
	return construct();
}

Base *ConstructorPython::_new() const
{
	return new Python(this);
}

void ConstructorPython::_delete(Base *base) const
{
	delete base;
}

QStringList ConstructorPython::extensions() const
{
	return QStringList() << "py" << "python";
}

ConstructorPython::ConstructorPython()
	: Constructor("Python Files")
{
}

Python::Python(const Constructor *constructor)
	: Base(new PythonLexer(), constructor)
{
}

Python::~Python()
{
	delete lexer();
}

QColor PythonLexer::defaultColor(int style) const
{
    switch (style)
    {
	case Default: 
	case ClassName:
	case FunctionMethodName:
	case Identifier:
	case HighlightedIdentifier:
		return lexer::Settings::ref().getSetting(DEFAULT, SyntaxStandards::defaultColor());

	case Comment:
	case CommentBlock:
		return lexer::Settings::ref().getSetting(COMMENT, SyntaxStandards::commentColor());

	case Number: return lexer::Settings::ref().getSetting(NUMBER, SyntaxStandards::numberColor());
	case Keyword: return lexer::Settings::ref().getSetting(KEYWORD, SyntaxStandards::keywordColor());

	case DoubleQuotedString:
	case SingleQuotedString:
	case TripleDoubleQuotedString:
	case TripleSingleQuotedString:
		return lexer::Settings::ref().getSetting(STRING, SyntaxStandards::stringColor());

	case Operator: return lexer::Settings::ref().getSetting(OPERATOR, SyntaxStandards::operatorColor());
	case UnclosedString: return lexer::Settings::ref().getSetting(UNCLOSED_STRING, SyntaxStandards::unclosedStringColor());
    }

    return QsciLexerPython::defaultColor(style);
}

QFont PythonLexer::font(int style) const
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

QFont PythonLexer::defaultFont(int style) const
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

const bool Python::cStyleBlocks() const
{
	return false;
}


