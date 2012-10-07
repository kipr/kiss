#include <QtGui>

#include "c_highlighter.hpp"
#include "syntax_standards.hpp"

using namespace Kiss;

CHighlighter::CHighlighter(QTextDocument *parent)
	: SyntaxHighlighter(parent)
{
	SyntaxHighlighter::Rule rule;

	QTextCharFormat keywordFormat;
	keywordFormat.setForeground(SyntaxStandards::keywordColor());
	keywordFormat.setFontWeight(QFont::Bold);
	
	QStringList keywordPatterns;
	keywordPatterns << "auto" << "break" << "case" << "char"
		<< "const" << "continue" << "default" << "do"
		<< "double" << "else" << "enum" << "extern"
		<< "float" << "for" << "goto" << "if" << "int"
		<< "long" << "register" << "return" << "short"
		<< "signed" << "sizeof" << "static" << "struct"
		<< "switch" << "typedef" << "union" << "unsigned"
		<< "void" << "volatile" << "while";
	
	foreach(const QString& pattern, keywordPatterns) {
		rule.pattern = QRegExp(QString("\\b") + pattern + "\\b");
		rule.format = keywordFormat;
		addRule(rule);
	}
	
	QTextCharFormat numberFormat;
	numberFormat.setForeground(SyntaxStandards::numberColor());
	
	rule.pattern = QRegExp("\\b\\d+((l|L)|(e\\d))?\\b");
	rule.format = numberFormat;
	addRule(rule);
	
	rule.pattern = QRegExp("\\b0x[0-9A-Fa-f]+\\b");
	rule.format = numberFormat;
	addRule(rule);
	
	QTextCharFormat preprocessor;
	preprocessor.setForeground(SyntaxStandards::preprocessorColor());
	rule.pattern = QRegExp("^#.+");
	rule.format = preprocessor;
	addRule(rule);

	QTextCharFormat quotationFormat;
	quotationFormat.setForeground(SyntaxStandards::stringColor());
	rule.pattern = QRegExp("\".*\"");
	rule.format = quotationFormat;
	addRule(rule);

	QTextCharFormat singleLineCommentFormat;
	singleLineCommentFormat.setForeground(SyntaxStandards::commentColor());
	rule.pattern = QRegExp("//[^\n]*");
	rule.format = singleLineCommentFormat;
	addRule(rule);

	QTextCharFormat multiLineCommentFormat;
	multiLineCommentFormat.setForeground(SyntaxStandards::commentColor());
	setMultiLineCommentFormat(multiLineCommentFormat);

	setCommentStartExpression(QRegExp("/\\*"));
	setCommentEndExpression(QRegExp("\\*/"));
}

