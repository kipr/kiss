#include "JavaLexerSpecProvider.h"
#include "LexerStyles.h"

#include <QColor>
#include <QFont>

JavaLexerSpecProvider::JavaLexerSpecProvider() : LexerSpecProvider("java")
{
	
}

void JavaLexerSpecProvider::init()
{
	m_lexerSpec.language = "Java";
	m_lexerSpec.lexer = LexerCPP::lexerName();
	m_lexerSpec.autoCompletionWordSeparators.clear();
	m_lexerSpec.autoCompletionWordSeparators << ".";
	m_lexerSpec.blockEnd = "}";
	m_lexerSpec.blockEndStyle = LexerCPP::Operator;
	m_lexerSpec.blockStart = "{";
	m_lexerSpec.blockStartStyle = LexerCPP::Operator;
	m_lexerSpec.blockStartKeyword = "case default";
	m_lexerSpec.blockStartKeywordStyle = LexerCPP::Keyword;
	m_lexerSpec.braceStyle = LexerCPP::Operator;
	m_lexerSpec.wordCharacters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_#";

	m_lexerSpec.defaultColor[LexerCPP::Comment] = QColor("green");
	m_lexerSpec.defaultColor[LexerCPP::CommentLine] = QColor("green");
	m_lexerSpec.defaultColor[LexerCPP::CommentDoc] = QColor("green");
	m_lexerSpec.defaultColor[LexerCPP::CommentLineDoc] = QColor("green");
	m_lexerSpec.defaultColor[LexerCPP::CommentDocKeyword] = QColor("green");
	m_lexerSpec.defaultColor[LexerCPP::CommentDocKeywordError] = QColor("green");
	m_lexerSpec.defaultColor[LexerCPP::Keyword] = QColor("darkBlue");
	m_lexerSpec.defaultColor[LexerCPP::String] = QColor("darkRed");
	m_lexerSpec.defaultColor[LexerCPP::Character] = QColor("darkRed");
	m_lexerSpec.defaultColor[LexerCPP::PreProcessor] = QColor("darkBlue");
	m_lexerSpec.defaultColor[LexerCPP::StringEol] = QColor("darkRed");

	m_lexerSpec.defaultFont[LexerCPP::Keyword] = QFont("", -1, QFont::Bold);
	m_lexerSpec.defaultFont[LexerCPP::Operator] = QFont("", -1, QFont::Bold);

	m_lexerSpec.keywords[1] = "break case char final continue default do "
		"double else enum float for goto if int long "
		"return short static "
		"switch void while "
		"class extends implements public protected private";
	
	m_lexerSpec.defaultColor[LexerCPP::Keyword2] = QColor("darkMagenta");
	m_lexerSpec.defaultFont[LexerCPP::Keyword2] = QFont("", -1, QFont::Bold);
	m_lexerSpec.keywords[2] = "boolean_expression  variable a_value starting_value ending_value change_in_variable";
}

Q_EXPORT_PLUGIN2(java_plugin, JavaLexerSpecProvider);