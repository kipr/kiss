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

#ifndef __LEXER_STYLES_H__
#define __LEXER_STYLES_H__

/* This file holds various classes with enums
 * representing the style names available for
 * the various Scintilla lexers.			  */

class LexerCPP
{
	public:
	enum {
		Default = 0,
		Comment = 1,
		CommentLine = 2,
		CommentDoc = 3,
		Number = 4,
		Keyword = 5,
		String = 6,
		Character = 7,
		UUID = 8,
		PreProcessor = 9,
		Operator = 10,
		Identifier = 11,
		StringEol = 12,
		Verbatim = 13,
		Regex = 14,
		CommentLineDoc = 15,
		Keyword2 = 16,
		CommentDocKeyword = 17,
		CommentDocKeywordError = 18,
		GlobalClass = 19,
		StyleCount = 20
	};
	static const char* lexerName() { return "cpp";};
};

#endif
