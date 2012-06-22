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

#include "LexerC.h"

#include <QDebug>
#include <QtPlugin>

#include "SyntaxStandards.h"

using namespace Lexer;

ConstructorC::ConstructorC() : Constructor("C Files") {}
LexerBase* ConstructorC::construct() const { return Factory::ref().newLexerFromConstructor(this); }
LexerBase* ConstructorC::construct(const QString& apis) const { return construct(); }
LexerBase* ConstructorC::_new() const { return new C(this); }
QStringList ConstructorC::extensions() const { return QStringList() << "c"; }

C::C(const Constructor* constructor) : LexerBase(this, constructor) {}

QColor C::defaultColor(int style) const
{
    switch (style)
    {
	case Default: return SyntaxStandards::defaultColor();

	case Comment:
	case CommentLine:
	case InactiveComment:
	case InactiveCommentLine:
		return SyntaxStandards::commentColor();

	case CommentDoc:
	case CommentLineDoc:
	case InactiveCommentDoc:
		return SyntaxStandards::docColor();

	case Number: return SyntaxStandards::numberColor();
	case Keyword: return SyntaxStandards::keywordColor();

	case DoubleQuotedString:
	case SingleQuotedString:
	case RawString:
		return SyntaxStandards::stringColor();

	case PreProcessor: return SyntaxStandards::preprocessorColor();
	case Operator: return SyntaxStandards::operatorColor();
	case UnclosedString: return SyntaxStandards::unclosedStringColor();
	case CommentDocKeyword: return SyntaxStandards::docKeywordColor();
	case CommentDocKeywordError: return SyntaxStandards::docKeywordErrorColor();

	case InactiveDefault:
	case InactiveUUID:
	case InactiveCommentLineDoc:
	case InactiveKeywordSet2:
	case InactiveCommentDocKeyword:
	case InactiveCommentDocKeywordError:
		return SyntaxStandards::inactiveColor();


	case InactiveNumber: return SyntaxStandards::inactiveNumberColor();
	case InactiveKeyword: return SyntaxStandards::inactiveKeywordColor();

	case InactiveDoubleQuotedString:
	case InactiveSingleQuotedString:
	case InactiveRawString:
		return SyntaxStandards::inactiveStringColor();

	case InactivePreProcessor: return SyntaxStandards::inactivePreprocessorColor();

	case InactiveOperator:
	case InactiveIdentifier:
	case InactiveGlobalClass:
		return SyntaxStandards::inactiveOperatorColor();

	case InactiveUnclosedString: return SyntaxStandards::inactiveUnclosedStringColor();
	case InactiveVerbatimString: return SyntaxStandards::inactiveVerbatimStringColor();
    }

    return QsciLexerCPP::defaultColor(style);
}

QFont C::font(int style) const
{
	QFont f = QsciLexer::font(style);

	switch (style)
	{
	case Keyword:
	case InactiveKeyword:
	case Operator:
	case InactiveOperator:
		f.setBold(true);
		break;
	}
	
	return f;
}

QFont C::defaultFont(int style) const
{
	QFont f = QsciLexer::defaultFont(style);

	switch (style)
	{
	case Keyword:
	case InactiveKeyword:
	case Operator:
	case InactiveOperator:
		f.setBold(true);
		break;
	}


	return f;
}

const char *C::keywords(int set) const
{
	if(set != 1) return 0;
	
	return 	"auto _Bool break case char _Complex const continue default do "
		"double else enum extern float for goto if _Imaginary inline "
		"int long register restrict return short signed sizeof static "
		"struct switch typedef union unsigned void volatile while";
}

const bool C::cStyleBlocks() const { return true; }

Q_EXPORT_PLUGIN2(cpp_lexer, Lexer::ConstructorC);