#ifndef __JAVA_LEXER_SPEC_PROVIDER_H__
#define __JAVA_LEXER_SPEC_PROVIDER_H__

#include "LexerSpecProvider.h"
#include <QObject>

class JavaLexerSpecProvider : public QObject, public LexerSpecProvider
{
	Q_OBJECT
	Q_INTERFACES(LexerSpecProvider)
public:
	JavaLexerSpecProvider();
	
	virtual void init();
};

#endif