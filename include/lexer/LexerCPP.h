#ifndef _LEXERCPP_H_
#define _LEXERCPP_H_

#include "LexerFactory.h"

#include <Qsci/qscilexercpp.h>
#include <Qsci/qsciapis.h>

namespace Lexer
{
	class CPP : public LexerBase
	{
	public:
		CPP(const Constructor* constructor);
		
		QColor defaultColor(int style) const;
		QFont font(int style) const;
		QFont defaultFont(int style) const;
	};
	
	struct ConstructorCPP : Constructor
	{
		LexerBase* construct() const { return Factory::ref().newLexerFromConstructor(this); }
		LexerBase* construct(const QString& apis) const { return construct(); }
		
		LexerBase* _new() const { return new CPP(this); }
	};
}

#endif
