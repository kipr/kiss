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

#ifndef _TEMPLATEFORMAT_H_
#define _TEMPLATEFORMAT_H_

#include "MacroString.h"

class QTextStream;

struct TemplateFormatReader;
struct TemplateFormatWriter;

class TemplateFormat
{
public:
	virtual void update() = 0;
	
	bool hasMetaData() const;
	bool hasLexerName() const;
	QString lexerName() const;
	QString content() const;
	
	void setHasMetaData(bool hasMetaData);
	void setHasLexerName(bool hasLexerName);
	void setLexerName(const QString& lexerName);
	void setContent(const QString& content);
	
	TemplateFormatReader* newTemplateReader() const;
	TemplateFormatWriter* newTemplateWriter() const;
protected:
	TemplateFormat(QTextStream* stream);
	QTextStream* m_stream;
	MacroString m_templateMacro;

private:
	bool m_hasMetaData;
	bool m_hasLexerName;
	QString m_lexerName;
	QString m_content;
};

struct TemplateFormatReader : TemplateFormat
{
	TemplateFormatReader(QTextStream* in);
	virtual void update();
};

struct TemplateFormatWriter : TemplateFormat
{
	TemplateFormatWriter(QTextStream* out);
	virtual void update();
};

#endif
