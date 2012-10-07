#include "syntax_highlighter.hpp"

#include <QSyntaxHighlighter>
#include <QRegExp>
#include <QStringList>
#include <QDebug>

using namespace Kiss;

SyntaxHighlighter::SyntaxHighlighter(QTextDocument *parent)
	: QSyntaxHighlighter(parent)
{
}

void SyntaxHighlighter::highlightBlock(const QString& text)
{
	qDebug() << "Highlighting" << text;
	foreach(const Rule& rule, m_highlightingRules) {
		QRegExp expression(rule.pattern);
		int index = expression.indexIn(text);
		while(index >= 0) {
			int length = expression.matchedLength();
			setFormat(index, length, rule.format);
			index = expression.indexIn(text, index + length);
		}
	}
	
	setCurrentBlockState(0);

	int startIndex = 0;
	if(previousBlockState() != 1)
		startIndex = m_commentStartExpression.indexIn(text);

	while(startIndex >= 0) {
		int endIndex = m_commentEndExpression.indexIn(text, startIndex);
		int commentLength;
		if(endIndex == -1) {
			setCurrentBlockState(1);
			commentLength = text.length() - startIndex;
		} else {
			commentLength = endIndex - startIndex + m_commentEndExpression.matchedLength();
		}
		setFormat(startIndex, commentLength, m_multiLineCommentFormat);
		startIndex = m_commentStartExpression.indexIn(text, startIndex + commentLength);
	}
}

void SyntaxHighlighter::setCommentStartExpression(const QRegExp& commentStartExpression)
{
	m_commentStartExpression = commentStartExpression;
}

void SyntaxHighlighter::setCommentEndExpression(const QRegExp& commentEndExpression)
{
	m_commentEndExpression = commentEndExpression;
}

void SyntaxHighlighter::setMultiLineCommentFormat(const QTextCharFormat& multiLineCommentFormat)
{
	m_multiLineCommentFormat = multiLineCommentFormat;
}

void SyntaxHighlighter::addRule(const SyntaxHighlighter::Rule& rule)
{
	m_highlightingRules.append(rule);
}

const QRegExp& SyntaxHighlighter::commentStartExpression() const
{
	return m_commentStartExpression;
}

const QRegExp& SyntaxHighlighter::commentEndExpression() const
{
	return m_commentEndExpression;
}

const QTextCharFormat& SyntaxHighlighter::multiLineCommentFormat() const
{
	return m_multiLineCommentFormat;
}

const QVector<SyntaxHighlighter::Rule>& SyntaxHighlighter::rules() const
{
	return m_highlightingRules;
}