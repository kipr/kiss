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

#ifndef __LEXER_H__
#define __LEXER_H__

#include "LexerSpec.h"

#include <QObject>
#include <Qsci/qscilexer.h>
#include <Qsci/qsciapis.h>

/** 
 * Lexer class inherits QsciLexer
 *
 * This class implements all of the methods provided by QsciLexer
 * and allows them to be customized through the use of a LexerSpec
 * object.  This object is static across all instances of the class
 * and is designed to be passed in by a plugin
 *
 */
class Lexer : public QsciLexer
{
	Q_OBJECT
public:
	// Constructor & Destructor 
	Lexer(LexerSpec* spec, QString api);
	~Lexer();

	// Implementations of the virtuals defined in QsciLexer
	const char *language() const;
	const char *lexer() const;
	QStringList autoCompletionWordSeparators() const;
	const char *blockEnd(int *style = 0) const;
	const char *blockStart(int *style = 0) const;
	const char *blockStartKeyword(int *style = 0) const;
	int braceStyle() const;
	const char *wordCharacters() const;
	QColor defaultColor(int style) const;
	bool defaultEolFill(int style) const;
	QFont defaultFont(int style) const;
	QColor defaultPaper(int style) const;
	const char *keywords(int set) const;
	QString description(int style) const;
private slots:
	void prepFinished();
	
private:
	LexerSpec* m_lexerSpec;
	QsciAPIs m_apis;
};

#endif
