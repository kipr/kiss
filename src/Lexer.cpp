/**************************************************************************
 *  Copyright 2007-2011 KISS Institute for Practical Robotics             *
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

#include "Lexer.h"

#include <QColor>
#include <QFont>
#include <QDebug>
#include <QSettings>
#include <Qsci/qsciapis.h>
#include <string.h>

Lexer::Lexer(LexerSpec* spec, QString api) : QsciLexer(0), m_lexerSpec(spec), m_apis(this)
{
	connect(&m_apis, SIGNAL(apiPreparationFinished()), this, SLOT(prepFinished()));
	m_apis.clear();
	if(!m_apis.load(api)) qWarning() << "Failed to load API" << api;
	else m_apis.prepare();
}

const char* Lexer::language() const { return m_lexerSpec->language.toLocal8Bit().data(); }

const char* Lexer::lexer() const { return m_lexerSpec->lexer.toLocal8Bit().data(); }

QStringList Lexer::autoCompletionWordSeparators() const { return m_lexerSpec->autoCompletionWordSeparators; }

const char* Lexer::blockStartKeyword(int* style) const
{
	if (style) *style = m_lexerSpec->blockStartKeywordStyle;
	return m_lexerSpec->blockStartKeyword.toLocal8Bit().data();
}

const char* Lexer::blockStart(int* style) const
{
	if (style) *style = m_lexerSpec->blockStartStyle;
	return m_lexerSpec->blockStart.toLocal8Bit().data();
}

const char* Lexer::blockEnd(int* style) const
{
	if (style) *style = m_lexerSpec->blockEndStyle;
	return m_lexerSpec->blockEnd.toLocal8Bit().data();
}

int Lexer::braceStyle() const { return m_lexerSpec->braceStyle; }
const char* Lexer::wordCharacters() const { return m_lexerSpec->wordCharacters.toLocal8Bit().data(); }

QColor Lexer::defaultColor(int style) const
{
	return m_lexerSpec->defaultColor.contains(style) ? m_lexerSpec->defaultColor[style] 
		: QsciLexer::defaultColor(style);
}

bool Lexer::defaultEolFill(int style) const
{
	return m_lexerSpec->defaultEolFill.contains(style) ? true : QsciLexer::defaultEolFill(style);
}

QFont Lexer::defaultFont(int style) const
{
	if(m_lexerSpec->defaultFont.contains(style)) {
		QFont f(m_lexerSpec->defaultFont[style]);
		f.setFamily(QsciLexer::defaultFont(style).family());
		f.setPointSize(QsciLexer::defaultFont(style).pointSize());
		return f;
	}
	
	return QsciLexer::defaultFont(style);
}

const char* Lexer::keywords(int set) const
{
	return m_lexerSpec->keywords.contains(set) ? (const char*)m_lexerSpec->keywords[set].toAscii() : 0;
}

QColor Lexer::defaultPaper(int style) const
{
	return m_lexerSpec->defaultPaper.contains(style) ? m_lexerSpec->defaultPaper[style] 
		: QsciLexer::defaultPaper(style);
}

QString Lexer::description(int) const { return " "; }

void Lexer::prepFinished() { setAPIs(&m_apis); }