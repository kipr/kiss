#ifndef _LEXERJAVA_H_
#define _LEXERJAVA_H_

#include "LexerCPP.h"

namespace Lexer
{
	class Java : public CPP
	{
	public:
		Java(const Constructor* constructor);
		
		const char *keywords(int set) const;
	};
	
	struct ConstructorJava : Constructor
	{
		LexerBase* construct() const { return Factory::ref().newLexerFromConstructor(this); }
		LexerBase* construct(const QString& apis) const { return construct(); }
		
		LexerBase* _new() const { return new Java(this); }
	};
}

#endif
