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

#include "Transaction.h"

using namespace TransactionType;

Transaction::Transaction(const QString& name, int type) : m_name(name), m_type(type) {}

const QString& Transaction::name() const { return m_name; }
const int Transaction::type() const { return m_type; }

const QVariantList& Transaction::data() const { return m_data; }

QString Transaction::successMessage() const { return m_name + ": success!"; }
QString Transaction::failureMessage() const { return m_name + ": FAILURE!"; }

Unknown::Unknown() : Transaction("Unknown", Transaction::Unknown) {}

CreateProject::CreateProject(const QString& name) : Transaction(name + " (create project)", Transaction::CreateProject)
{
	m_data = QVariantList() << QVariant(name);
}

MakeDirectory::MakeDirectory(const QString& path) : Transaction(path + " (mkdir)", Transaction::MakeDirectory)
{
	m_data = QVariantList() << QVariant(path);
}

SendToDirectory::SendToDirectory(const QString& path, const QString& directory)
	 : Transaction(path + " -> " + directory, Transaction::SendToDirectory)
{
	m_data = QVariantList() << QVariant(path) << directory;
}

CompileFile::CompileFile(const QString& path, const QString& options)
	: Transaction(path + " (compile with options:" + options + ")", Transaction::CompileFile)
{
	
}

RunFile::RunFile(const QString& path, const QString& options)
	: Transaction(path + " (run with options:" + options + ")", Transaction::RunFile)
{
	
}
