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

#include "lexer_java.hpp"

#include "syntax_standards.hpp"

#include <QtPlugin>

using namespace Kiss::Lexer;

ConstructorJava::ConstructorJava()
	: Constructor("Java Files")
{}

Base *ConstructorJava::construct() const
{
	return Factory::ref().newLexerFromConstructor(this);
}

Base *ConstructorJava::construct(const QString& apis) const
{
	return construct();
}

Base *ConstructorJava::_new() const
{
	return new Java(this);
}

void ConstructorJava::_delete(Base *base) const
{
	delete base;
}

QStringList ConstructorJava::extensions() const
{
	return QStringList() << "java";
}

Java::Java(const Constructor* constructor)
	: Base(new JavaLexer(), constructor)
{
}

Java::~Java()
{
	delete lexer();
}

QColor JavaLexer::defaultColor(int style) const
{
    switch (style)
    {
	case Default: return Lexer::Settings::ref().getSetting(DEFAULT, SyntaxStandards::defaultColor());

	case Comment:
	case CommentLine:
	case InactiveComment:
	case InactiveCommentLine:
		return Lexer::Settings::ref().getSetting(COMMENT, SyntaxStandards::commentColor());

	case CommentDoc:
	case CommentLineDoc:
	case InactiveCommentDoc:
		return Lexer::Settings::ref().getSetting(DOC_COMMENT, SyntaxStandards::docColor());

	case Number: return Lexer::Settings::ref().getSetting(NUMBER, SyntaxStandards::numberColor());
	case Keyword: return Lexer::Settings::ref().getSetting(KEYWORD, SyntaxStandards::keywordColor());

	case DoubleQuotedString:
	case SingleQuotedString:
	case RawString:
		return Lexer::Settings::ref().getSetting(STRING, SyntaxStandards::stringColor());

	case PreProcessor: return Lexer::Settings::ref().getSetting(PREPROCESSOR, SyntaxStandards::preprocessorColor());
	case Operator: return Lexer::Settings::ref().getSetting(OPERATOR, SyntaxStandards::operatorColor());
	case UnclosedString: return Lexer::Settings::ref().getSetting(UNCLOSED_STRING, SyntaxStandards::unclosedStringColor());
	case CommentDocKeyword: return Lexer::Settings::ref().getSetting(DOC_KEYWORD, SyntaxStandards::docKeywordColor());
	case CommentDocKeywordError: return Lexer::Settings::ref().getSetting(DOC_KEYWORD_ERROR, SyntaxStandards::docKeywordErrorColor());

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

QFont JavaLexer::font(int style) const
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

QFont JavaLexer::defaultFont(int style) const
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

const bool Java::cStyleBlocks() const
{
	return true;
}

Q_EXPORT_PLUGIN2(java_lexer, Kiss::Lexer::ConstructorJava);
