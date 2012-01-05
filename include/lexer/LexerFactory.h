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

#ifndef _LEXERFACTORY_H_
#define _LEXERFACTORY_H_

#include "Singleton.h"

#include <QMap>
#include <QFont>
#include <Qsci/qscilexercpp.h>
#include <Qsci/qsciapis.h>

#include <QDebug>

namespace Lexer
{
	class LexerBase;
	
	struct Constructor
	{
		virtual LexerBase* construct() const = 0;
		virtual LexerBase* construct(const QString& apis) const = 0;
		
		virtual LexerBase* _new() const = 0;
	};

	struct LexerBase : public QsciLexerCPP
	{
		LexerBase(const Constructor* constructor) : m_constructor(constructor), m_apis(this) {}
		const Constructor* constructor() const { return m_constructor; }
		
		void setAPIFile(const QString& apis) { if(m_apis.load(apis)) m_apis.prepare(); }
		
	private:
		const Constructor* m_constructor;
		QsciAPIs m_apis;
	};

	class Factory : public Singleton<Factory>
	{
	public:
		Factory();
		~Factory();
	
		LexerBase* newLexer(const QString& ext) const;
		LexerBase* newLexerFromConstructor(const Constructor* constructor) const;
		Constructor* constructor(const QString& ext) const;
	
		void registerLexerConstructor(Constructor* c, const QStringList& exts);
		
		static void setAPIsForLexer(LexerBase* lexer, const QString& apis);
		static bool isLexerFromConstructor(LexerBase* lexer, Constructor* constructor);
	
		void setFont(const QFont& font);
		QFont font() const;
	private:
		QFont m_font;
		QMap<QString, Constructor*> m_constructors;
	};
}

#endif
