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

#ifndef _LEXER_JAVASCRIPT_HPP_
#define _LEXER_JAVASCRIPT_HPP_

#include "lexer_factory.hpp"

#include <Qsci/qscilexerjavascript.h>
#include <Qsci/qsciapis.h>

namespace Kiss
{
	namespace Lexer
	{
		class JavaScript : public Base, public QsciLexerJavaScript
		{
		public:
			JavaScript(const Constructor* constructor);

			virtual const bool cStyleBlocks() const;
		};

		class ConstructorJavaScript : public Constructor
		{
		Q_OBJECT
		Q_INTERFACES(Lexer::Constructor)
		public:
			ConstructorJavaScript();

			LexerBase* construct() const;
			LexerBase* construct(const QString& apis) const;

			LexerBase* _new() const;
			void _delete(Base *base) const;

			QStringList extensions() const;
		};
	}
}

#endif
