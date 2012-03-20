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

#ifndef _LEXERCPP_H_
#define _LEXERCPP_H_

#include "LexerFactory.h"

#include <Qsci/qscilexercpp.h>
#include <Qsci/qsciapis.h>

namespace Lexer
{
	class CPP : public LexerBase, virtual public QsciLexerCPP
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
