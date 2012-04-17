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

#include "LexerFactory.h"

#include "LexerC.h"
#include "LexerCPP.h"
#include "LexerJava.h"
#include "LexerJavaScript.h"

#include <QDebug>

using namespace Lexer;

Factory::Factory()
{
	registerLexerConstructor(new ConstructorJava(), QStringList() << "java");
	registerLexerConstructor(new ConstructorC(), QStringList() << "c");
	registerLexerConstructor(new ConstructorCPP(), QStringList() << "cpp" << "h" << "hpp" << "cxx");
	registerLexerConstructor(new ConstructorJavaScript(), QStringList() << "js" << "javascript");
}

Factory::~Factory()
{
	QList<Constructor*> removed;
	const QList<Constructor*>& l = m_constructors.values();
	foreach(Constructor* c, l) {
		if(removed.contains(c)) continue;
		
		delete c;
		removed.append(c);
	}
}

LexerBase* Factory::newLexer(const QString& name) const
{
	return constructor(name)->construct();
}

LexerBase* Factory::newLexerFromConstructor(const Constructor* constructor) const
{
	LexerBase* ret = constructor->_new();
	ret->lexer()->setFont(m_font, -1);
	return ret;
}

Constructor* Factory::constructor(const QString& ext) const
{
	QMap<QString, Constructor*>::const_iterator i = m_constructors.find(ext);
	if(i == m_constructors.end()) return 0;
	
	return i.value();
}

void Factory::registerLexerConstructor(Constructor* c, const QStringList& exts)
{
	foreach(const QString& ext, exts) m_constructors[ext] = c;
}

void Factory::setAPIsForLexer(LexerBase* lexer, const QString& apis)
{
	if(!lexer) return;
	lexer->setAPIFile(apis);
}

bool Factory::isLexerFromConstructor(LexerBase* lexer, Constructor* constructor)
{
	if(!lexer) return false;
	return lexer->constructor() == constructor;
}

void Factory::setFont(const QFont& font)
{
	m_font = font;
}

QFont Factory::font() const
{
	return m_font;
}