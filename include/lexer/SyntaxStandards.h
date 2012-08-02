#ifndef _SYNTAXSTANDARDS_H_
#define _SYNTAXSTANDARDS_H_

#include <QColor>

#define DEFAULT "Default"
#define COMMENT "Comments"
#define DOC_COMMENT "Doc Comments"
#define NUMBER "Numbers"
#define KEYWORD "Keywords"
#define STRING "Strings"
#define PREPROCESSOR "Preprocessors"
#define OPERATOR "Operators"
#define UNCLOSED_STRING "Unclosed Strings"
#define DOC_KEYWORD "Doc Keywords"
#define DOC_KEYWORD_ERROR "Doc Keyword Errors"

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
