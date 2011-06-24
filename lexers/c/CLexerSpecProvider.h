#ifndef __C_LEXER_SPEC_PROVIDER_H__
#define __C_LEXER_SPEC_PROVIDER_H__

#include "LexerSpecProvider.h"
#include <QObject>

class CLexerSpecProvider : public QObject, public LexerSpecProvider
{
	Q_OBJECT
	Q_INTERFACES(LexerSpecProvider)
public:
	CLexerSpecProvider();
	
	virtual void init();
};

#endif