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

// Note: This file actually isn't used yet. It is in preparation for KISS IDE 3.1.0

#ifndef _TRANSACTION_H_
#define _TRANSACTION_H_

#include <QList>
#include <QVariant>

#define CREATED_PROJECT_PATH QString("CREATED_PROJECT_PATH")

typedef QList<QVariant> QVariantList;

class Transaction
{
public:
	Transaction(const QString& name, int type);
	
	const QString& name() const;
	const int type() const;
	const QVariantList& data() const;
	
	virtual QString successMessage() const;
	virtual QString failureMessage() const;
	
	enum {
		Unknown = 0,
		CreateProject,
		MakeDirectory,
		SendToDirectory,
		CompileFile,
		RunFile
	};
	
protected:
	QVariantList m_data;

private:
	QString m_name;
	int m_type;
};

namespace TransactionType
{
	struct Unknown : Transaction
	{
		Unknown();
	};

	struct CreateProject : Transaction
	{
		CreateProject(const QString& name);
	};
	
	struct MakeDirectory : Transaction
	{
		MakeDirectory(const QString& path);
	};
	
	struct SendToDirectory : Transaction
	{
		SendToDirectory(const QString& path, const QString& directory);
	};
	
	struct CompileFile : Transaction
	{
		CompileFile(const QString& path, const QString& options);
	};
	
	struct RunFile : Transaction
	{
		RunFile(const QString& path, const QString& options);
	};
}

typedef QList<Transaction*> TransactionList;

#endif
