#ifndef _C_HIGHLIGHTER_HPP_
#define _C_HIGHLIGHTER_HPP_

#include "syntax_highlighter.hpp"

#include <QHash>

class QTextDocument;

namespace Kiss
{
	class CHighlighter : public SyntaxHighlighter
	{
	Q_OBJECT
	public:
		CHighlighter(QTextDocument *parent = 0);
	};
}

#endif