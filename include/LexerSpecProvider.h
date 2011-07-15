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

#ifndef __LEXER_SPEC_PROVIDER_H__
#define __LEXER_SPEC_PROVIDER_H__

#include "LexerSpec.h"

#include <qplugin.h>
#include <QString>

class LexerSpecProvider
{
public:
	LexerSpecProvider(const QString& extension) : m_lexerSpec(), m_extension(extension) {}
	
	virtual void init() = 0;
	QString extension() const { return m_extension; }
	
	LexerSpec* lexerSpec() { return &m_lexerSpec; }
	
protected:
	LexerSpec m_lexerSpec;
	QString m_extension;
};

Q_DECLARE_INTERFACE(LexerSpecProvider, "com.kipr.kiss-c.LexerSpecProvider/1.0");

#endif