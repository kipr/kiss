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

#include "template_format.hpp"

#include "macro_string.hpp"

#include <QTextStream>

#define KISS_LEXER "KISS_LEXER"
#define KISS_DATE "KISS_DATE"
#define END_KISS "END_KISS_META"

TemplateFormat::TemplateFormat(QTextStream* stream)
	: m_stream(stream), m_hasMetaData(false),
	m_hasLexerName(false), m_lexerName(""), m_content("")
{
	m_templateMacro[KISS_DATE] = new DateMacro();
}

bool TemplateFormat::hasMetaData() const { return m_hasMetaData; }
bool TemplateFormat::hasLexerName() const { return m_hasLexerName; }
QString TemplateFormat::lexerName() const { return m_lexerName; }
QString TemplateFormat::content() const { return m_content; }

void TemplateFormat::setHasMetaData(bool hasMetaData) { m_hasMetaData = true; }
void TemplateFormat::setHasLexerName(bool hasLexerName) { setHasMetaData(hasLexerName); m_hasLexerName = true; }
void TemplateFormat::setLexerName(const QString& lexerName) { setHasLexerName(!lexerName.isEmpty()); m_lexerName = lexerName; }
void TemplateFormat::setContent(const QString& content) { m_content = content; }

TemplateFormatReader* TemplateFormat::newTemplateReader() const
{
	TemplateFormatReader* ret = new TemplateFormatReader(m_stream);
	ret->setHasMetaData(m_hasMetaData);
	ret->setHasLexerName(m_hasLexerName);
	ret->setLexerName(m_lexerName);
	ret->setContent(m_content);
	
	return ret;
}

TemplateFormatWriter* TemplateFormat::newTemplateWriter() const
{
	TemplateFormatWriter* ret = new TemplateFormatWriter(m_stream);
	ret->setHasMetaData(m_hasMetaData);
	ret->setHasLexerName(m_hasLexerName);
	ret->setLexerName(m_lexerName);
	ret->setContent(m_content);
	
	return ret;
}

TemplateFormatReader::TemplateFormatReader(QTextStream* in) : TemplateFormat(in) { update(); }
TemplateFormatWriter::TemplateFormatWriter(QTextStream* out) : TemplateFormat(out) {}

void TemplateFormatReader::update()
{
	const QString& str = m_stream->readAll();
	
	if(m_templateMacro.macroExists(str, KISS_LEXER, END_KISS))
		setLexerName(m_templateMacro.macroArguments(str, KISS_LEXER, END_KISS)[0]);
	
	setContent(m_templateMacro.apply(m_templateMacro.nonMeta(str, END_KISS)).trimmed() + "\n");
}

void TemplateFormatWriter::update()
{
	QTextStream& stream = *m_stream;
	if(hasMetaData()) {
		if(hasLexerName()) {
			stream << KISS_LEXER;
			stream <<  " ";
			stream << lexerName();
			stream <<  "\n";
		}
		stream << END_KISS << "\n";
	}
	
	stream << content();
}