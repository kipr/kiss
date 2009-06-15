/**************************************************************************
 *  Copyright 2007,2008,2009 KISS Institute for Practical Robotics        *
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
#include <QSettings>
#include <Qsci/qsciapis.h>

#include <string.h>

#include "LexerSpec.h"

bool Lexer::sm_hasLexerSpec = false;
LexerSpec Lexer::sm_lexerSpec;
QsciAPIs *Lexer::sm_apis = 0;

// Ugly constructor which initializes all of the local member variables
Lexer::Lexer(QObject *parent) : QsciLexer(parent)
{
	m_language = new char[sm_lexerSpec.language.length()+1];
	strncpy(m_language, sm_lexerSpec.language.toLocal8Bit(), sm_lexerSpec.language.length()+1);
	
	m_lexer = new char[sm_lexerSpec.lexer.length()+1];
	strncpy(m_lexer, sm_lexerSpec.lexer.toLocal8Bit(), sm_lexerSpec.lexer.length()+1);
	
	m_autoCompletionWordSeparators = sm_lexerSpec.autoCompletionWordSeparators;
	
	m_blockEnd = new char[sm_lexerSpec.blockEnd.length()+1];
	strncpy(m_blockEnd, sm_lexerSpec.blockEnd.toLocal8Bit(), sm_lexerSpec.blockEnd.length()+1);
	m_blockEndStyle = sm_lexerSpec.blockEndStyle;
	
	m_blockStart = new char[sm_lexerSpec.blockStart.length()+1];
	strncpy(m_blockStart, sm_lexerSpec.blockStart.toLocal8Bit(), sm_lexerSpec.blockStart.length()+1);
	m_blockStartStyle = sm_lexerSpec.blockStartStyle;
	
	m_blockStartKeyword = new char[sm_lexerSpec.blockStartKeyword.length()+1];
	strncpy(m_blockStartKeyword, sm_lexerSpec.blockStartKeyword.toLocal8Bit(), sm_lexerSpec.blockStartKeyword.length()+1);
	m_blockStartKeywordStyle = sm_lexerSpec.blockStartKeywordStyle;
	
	m_braceStyle = sm_lexerSpec.braceStyle;
	
	m_wordCharacters = new char[sm_lexerSpec.wordCharacters.length()+1];
	strncpy(m_wordCharacters, sm_lexerSpec.wordCharacters.toLocal8Bit(), sm_lexerSpec.wordCharacters.length()+1);
	
	m_defaultColor = sm_lexerSpec.defaultColor;
	m_defaultEolFill = sm_lexerSpec.defaultEolFill;
	m_defaultFont = sm_lexerSpec.defaultFont;
	m_defaultPaper = sm_lexerSpec.defaultPaper;
	m_keywords = sm_lexerSpec.keywords;

	setAPIs(sm_apis);
}

// Frees the memory allocated in the constructor
Lexer::~Lexer()
{
	delete m_language;
	delete m_lexer;
	delete m_blockEnd;
	delete m_blockStart;
	delete m_blockStartKeyword;
	delete m_wordCharacters;
}

const char *Lexer::language() const
{
	return m_language;
}

const char *Lexer::lexer() const
{
	return m_lexer;
}

QStringList Lexer::autoCompletionWordSeparators() const
{
    return m_autoCompletionWordSeparators;
}

const char *Lexer::blockStartKeyword(int *style) const
{
    if (style)
        *style = m_blockStartKeywordStyle;

    return m_blockStartKeyword;
}

const char *Lexer::blockStart(int *style) const
{
    if (style)
        *style = m_blockStartStyle;

    return m_blockStart;
}

const char *Lexer::blockEnd(int *style) const
{
    if (style)
        *style = m_blockEndStyle;

    return m_blockEnd;
}

int Lexer::braceStyle() const
{
    return m_braceStyle;
}

const char *Lexer::wordCharacters() const
{
	return m_wordCharacters;
}

QColor Lexer::defaultColor(int style) const
{
	if(m_defaultColor.contains(style)) {
		return  m_defaultColor[style];
	}

    return QsciLexer::defaultColor(style);
}

bool Lexer::defaultEolFill(int style) const
{
    if (m_defaultEolFill.contains(style))
        return true;

    return QsciLexer::defaultEolFill(style);
}

QFont Lexer::defaultFont(int style) const
{
    QFont f;

	if(m_defaultFont.contains(style)) {
		f = m_defaultFont[style];
		f.setFamily(QsciLexer::defaultFont(style).family());
		f.setPointSize(QsciLexer::defaultFont(style).pointSize());
	}
	else
		f = QsciLexer::defaultFont(style);

	return f;
}

const char *Lexer::keywords(int set) const
{
	if(m_keywords.contains(set))
		return m_keywords[set].toLocal8Bit();

	return 0;
}

QColor Lexer::defaultPaper(int style) const
{
	if(m_defaultPaper.contains(style))
		return m_defaultPaper[style];

    return QsciLexer::defaultPaper(style);
}

QString Lexer::description(int) const
{
	return " ";
}

void Lexer::setLexerSpec(LexerSpec *spec)
{
	if(spec) {
		sm_lexerSpec = *spec;
		sm_hasLexerSpec = true;
	}
	else {
		sm_lexerSpec = LexerSpec();
		sm_hasLexerSpec = false;
	}
}

void Lexer::setAPIFile(QString fileName)
{
	static Lexer lex;
	static QsciAPIs apis(&lex);
	sm_apis = &apis;

	sm_apis->clear();
	sm_apis->load(fileName);
	sm_apis->prepare();
}

bool Lexer::hasLexerSpec()
{
	return sm_hasLexerSpec;
}
