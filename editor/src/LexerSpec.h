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

#ifndef __LEXER_SPEC_H__
#define __LEXER_SPEC_H__

#include <QString>
#include <QStringList>
#include <QMap>
#include <QList>
#include <QColor>
#include <QFont>

typedef struct LexerSpecStruct {
	// Language as accepted by the chosen lexer (ex: "C")
	QString 	language;

	// Lexer as accepted by QsciLexer (ex: "cpp"
	QString 	lexer;

	// A list of auto completion word separators
	QStringList autoCompletionWordSeparators;

	// A QString with the block End character (ex: "}")
	QString 	blockEnd;

	// The style of the block end character
	int 		blockEndStyle;

	// A QString holding the block start character
	QString		blockStart;

	// The style of the block start character
	int			blockStartStyle;
	
	// A space separated list of keywords that can start a block
	QString		blockStartKeyword;

	// The style of the previously mentioned blockStartKeyword list
	int			blockStartKeywordStyle;

	// The stype of braces in this lexer
	int			braceStyle;

	// A string containing word characters. 
	// recommend: "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_#"
	QString		wordCharacters;

	// Set defaultColor[style]=QColor() to set the default color
	// for Any particular style 
	QMap<int, QColor> defaultColor;

	// The default styles in which the back ground should 
	// fill the rest of the Line if a newline is in this style
	QList<int>	defaultEolFill;

	// The default font attributes to be used for each style
	// Note point size and font family will NOT be used from here
	// since those can be set in the editor preferences
	// Mainly used to decide which styles to bold face
	QMap<int, QFont> defaultFont;

	// The default paper color for each style
	QMap<int, QColor> defaultPaper;

	// The various sets of keywords available (usually just use set 1)
	QMap<int, QString> keywords;
} LexerSpec;

#endif
