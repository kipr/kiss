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

#include <QObject>
#include <Qsci/qscilexer.h>
#include <Qsci/qsciapis.h>

#include <QString>
#include <QStringList>
#include <QMap>
#include <QList>
#include <QColor>
#include <QFont>
#include <qplugin.h>
#include <QString>

struct LexerCPP
{
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

/*! \struct LexerSpec
 * \brief Contains everything necessary to highlight a language
 * 
 * These are to be implemented by a LexerProvider, then loaded at runtime
 */
struct LexerSpec {
	QStringList extensions;
	
	QString language;
	/*! Internal lexer as identified by QScintilla */
	QString lexer;
	QStringList autoCompletionWordSeparators;
	QString blockEnd;
	int blockEndStyle;
	QString	blockStart;
	int blockStartStyle;
	QString	blockStartKeyword;
	int blockStartKeywordStyle;
	int braceStyle;
	QString	wordCharacters;
	QMap<int, QColor> defaultColor;
	QList<int> defaultEolFill;
	QMap<int, QFont> defaultFont;
	QMap<int, QColor> defaultPaper;
	QMap<int, QString> keywords;
};

/*! \class Lexer 
 * \brief Interfaces LexerSpec with QScintilla's QsciLexer.
 *
 * Wraps LexerSpec struct in QsciLexer, allowing LexerSpecs to be loaded into the SourceFile editor.
 */
class Lexer : public QsciLexer
{
	Q_OBJECT
public:
	Lexer(LexerSpec* spec, QString api);

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
