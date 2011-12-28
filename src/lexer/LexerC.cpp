#include "LexerC.h"

using namespace Lexer;

C::C(const Constructor* constructor) : CPP(constructor) {}

const char *C::keywords(int set) const
{
	if(set != 1) return 0;
	
	return 	"auto _Bool break case char _Complex const continue default do "
		"double else enum extern float for goto if _Imaginary inline "
		"int long register restrict return short signed sizeof static "
		"struct switch typedef union unsigned void volatile while";
}