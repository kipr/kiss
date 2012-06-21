#ifndef _LEXERSCRIPT_H_
#define _LEXERSCRIPT_H_

#include "LexerFactory.h"

#include <Qsci/qscilexerbase.h>
#include <Qsci/qsciapis.h>

namespace Lexer
{
	class Script : public LexerBase, public QsciLexerBase
	{
		virtual const bool cStyleBlocks() const { return false; }
	};
	
	struct ConstructorScript : Constructor
	{
		ConstructorScript() : Constructor("???") {}
		
		LexerBase* construct() const { return Factory::ref().newLexerFromConstructor(this); }
		LexerBase* construct(const QString& apis) const { return construct(); }
		
		LexerBase* _new() const { return new Script(this); }
	};
}

#endif
