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

#include "Compilers.h"

#include "Compiler.h"

Compilers::Compilers()
{
	
}

void Compilers::addCompiler(Compiler* compiler)
{
	if(m_compilers.contains(compiler)) return;
	m_compilers.append(compiler);
}

void Compilers::removeCompiler(Compiler* compiler)
{
	m_compilers.removeAll(compiler);
}

Compiler* Compilers::compiler(const QString& type) const
{
	foreach(Compiler* compiler, m_compilers)
		if(compiler->types().contains(type)) return compiler;
	
	return 0;
}

QList<Compiler*> Compilers::compilers(const QString& type) const
{
	QList<Compiler*> ret;
	foreach(Compiler* compiler, m_compilers)
		if(compiler->types().contains(type)) ret << compiler;
	
	return ret;
}

const QList<Compiler*>& Compilers::compilers() const
{
	return m_compilers;
}