#ifndef __CPP_LEXER_SPEC_PROVIDER_H__
#define __CPP_LEXER_SPEC_PROVIDER_H__

#include "LexerSpecProvider.h"
#include <QObject>

class CppLexerSpecProvider : public QObject, public LexerSpecProvider
{
	Q_OBJECT
	Q_INTERFACES(LexerSpecProvider)
public:
	CppLexerSpecProvider();
	
	virtual void init();
};

#endif