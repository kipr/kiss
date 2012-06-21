/**************************************************************************
 *  Copyright 2007-2012 KISS Institute for Practical Robotics             *
 *                                                                        *
 *  This file is part of KISS (Kipr's Instructional Software System).     *
 *                                                                        *
 *  KISS is free software: you can redistribute it and/or modify          *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation, either version 2 of the License, or     *
 *  (at your option) any later version.                                   *
 *                                                                        *
 *  KISS is distributed in the hope that it will be useful,               *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *  GNU General Public License for more details.                          *
 *                                                                        *
 *  You should have received a copy of the GNU General Public License     *
 *  along with KISS.  Check the LICENSE file in the project root.         *
 *  If not, see <http://www.gnu.org/licenses/>.                           *
 **************************************************************************/

#include "LexerC.h"

using namespace Lexer;

ConstructorC::ConstructorC() : Constructor("C Files") {}
LexerBase* ConstructorC::construct() const { return Factory::ref().newLexerFromConstructor(this); }
LexerBase* ConstructorC::construct(const QString& apis) const { return construct(); }
LexerBase* ConstructorC::_new() const { return new C(this); }

C::C(const Constructor* constructor) : CPP(constructor) {}

const char *C::keywords(int set) const
{
	if(set != 1) return 0;
	
	return 	"auto _Bool break case char _Complex const continue default do "
		"double else enum extern float for goto if _Imaginary inline "
		"int long register restrict return short signed sizeof static "
		"struct switch typedef union unsigned void volatile while";
}