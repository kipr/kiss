#ifndef _SYNTAXSTANDARDS_H_
#define _SYNTAXSTANDARDS_H_

#include <QColor>

#define DEFAULT "Default"
#define COMMENTS "Comments"
#define DOC_COMMENTS "Doc Comments"
#define NUMBERS "Numbers"
#define KEYWORDS "Keywords"
#define STRINGS "Strings"
#define PREPROCESSORS "Preprocessors"
#define OPERATORS "Operators"
#define UNCLOSED_STRINGS "Unclosed Strings"

class SyntaxStandards
{
public:
	static QColor defaultColor();
	static QColor commentColor();
	static QColor docColor();
	static QColor numberColor();
	static QColor keywordColor();
	static QColor stringColor();
	static QColor preprocessorColor();
	static QColor operatorColor();
	static QColor unclosedStringColor();
	static QColor docKeywordColor();
	static QColor docKeywordErrorColor();
	static QColor inactiveColor();
	static QColor inactiveNumberColor();
	static QColor inactiveKeywordColor();
	static QColor inactiveStringColor();
	static QColor inactivePreprocessorColor();
	static QColor inactiveOperatorColor();
	static QColor inactiveUnclosedStringColor();
	static QColor inactiveVerbatimStringColor();
};

#endif
