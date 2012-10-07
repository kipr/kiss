#ifndef _SYNTAX_HIGHLIGHTER_HPP_
#define _SYNTAX_HIGHLIGHTER_HPP_

#include <QSyntaxHighlighter>

#include <QHash>
#include <QTextCharFormat>

class QTextDocument;

namespace Kiss
{
	class SyntaxHighlighter : public QSyntaxHighlighter
	{
	Q_OBJECT
	public:
		struct Rule
		{
			QRegExp pattern;
			QTextCharFormat format;
		};
		
		const QRegExp& commentStartExpression() const;
		const QRegExp& commentEndExpression() const;
		const QTextCharFormat& multiLineCommentFormat() const;
		const QVector<Rule>& rules() const;
		
	protected:
		SyntaxHighlighter(QTextDocument *parent = 0);
		
		virtual void highlightBlock(const QString& text);
		
		void setCommentStartExpression(const QRegExp& commentStartExpression);
		void setCommentEndExpression(const QRegExp& commentEndExpression);
		void setMultiLineCommentFormat(const QTextCharFormat& multiLineCommentFormat);
		
		void addRule(const SyntaxHighlighter::Rule& rule);
	private:
		QVector<Rule> m_highlightingRules;

		QRegExp m_commentStartExpression;
		QRegExp m_commentEndExpression;
		QTextCharFormat m_multiLineCommentFormat;
	};
}


#endif