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

#include "lexer_factory.hpp"

#include <QDebug>

using namespace Kiss::Lexer;

Constructor::Constructor(const QString& name)
	: m_name(name)
{
	
}

const QString& Constructor::name() const
{
	return m_name;
}

Constructor::~Constructor() {}

Base::~Base() {}

Settings::Settings() {}

Settings::~Settings() {}

Factory::Factory() {}

Factory::~Factory()
{
	QList<Constructor *> removed;
	const QList<Constructor *>& l = m_constructors.values();
	foreach(Constructor *c, l) {
		if(removed.contains(c)) continue;
		
		delete c;
		removed.append(c);
	}
}

Base *Factory::newLexer(const QString& name) const
{
	return constructor(name)->construct();
}

Base *Factory::newLexerFromConstructor(const Constructor *constructor) const
{
	Base *ret = constructor->_new();
	ret->lexer()->setFont(m_font, -1);
	return ret;
}

Constructor *Factory::constructor(const QString& ext) const
{
	QMap<QString, Constructor*>::const_iterator i = m_constructors.find(ext);
	if(i == m_constructors.end()) return 0;
	
	return i.value();
}

void Factory::registerLexerConstructor(Constructor *c)
{
	registerLexerConstructor(c, c->extensions());
}

void Factory::registerLexerConstructor(Constructor *c, const QStringList& exts)
{
	foreach(const QString& ext, exts) {
		m_constructors[ext] = c;
	}
}

void Factory::unregisterLexerConstructor(Constructor *c)
{
	const QList<QString>& keys = m_constructors.keys();
	foreach(const QString& key, keys) {
		if(m_constructors.value(key) == c) m_constructors.remove(key);
	}
}

void Factory::setAPIsForLexer(Base *lexer, const QString& apis)
{
	if(!lexer) return;
	lexer->setAPIFile(apis);
}

bool Factory::isLexerFromConstructor(Base *lexer, Constructor *constructor)
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

const QStringList Factory::extensions() const
{
	QStringList exts(m_constructors.keys());
	exts.removeDuplicates();
	return exts;
}

const QStringList Factory::formattedExtensions() const
{
	QStringList ret;
	const QStringList& exts = extensions();
	QMap<QString, QStringList> types;
	foreach(const QString& ext, exts) types[constructor(ext)->name()] << "*." + ext;
	foreach(const QString& type, types.keys()) ret << type + " (" + types[type].join(" ") + ")";
	return ret;
}
