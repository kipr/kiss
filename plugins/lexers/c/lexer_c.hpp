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

#ifndef _LEXER_C_HPP_
#define _LEXER_C_HPP_

#include "lexer_factory.hpp"

#include <Qsci/qscilexercpp.h>
#include <Qsci/qsciapis.h>

namespace Kiss
{
	namespace Lexer
	{
		class CLexer : public QsciLexerCPP
		{
		public:
			QColor defaultColor(int style) const;
			QFont font(int style) const;
			QFont defaultFont(int style) const;
			const char *keywords(int set) const;
		};

		class C : public Base
		{
		public:
			C(const Constructor *constructor);
			~C();

			const bool cStyleBlocks() const;
		private:
			CLexer *m_lexer;
		};

		class ConstructorC : public Constructor
		{
		Q_OBJECT
		Q_INTERFACES(Kiss::Lexer::Constructor)
		public:
			ConstructorC();
		
			Base *construct() const;
			Base *construct(const QString& apis) const;
		
			Base *_new() const;
			void _delete(Base *base) const;
		
			QStringList extensions() const;
		};
	}
}

#endif
