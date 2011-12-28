#ifndef _LEXERC_H_
#define _LEXERC_H_

#include "LexerCPP.h"

namespace Lexer
{
	class C : public CPP
	{
	public:
		C(const Constructor* constructor);
		
		const char *keywords(int set) const;
	};

	struct ConstructorC : Constructor
	{
		LexerBase* construct() const { return Factory::ref().newLexerFromConstructor(this); }
		LexerBase* construct(const QString& apis) const { return construct(); }
		
		LexerBase* _new() const { return new C(this); }
	};
}

#endif
