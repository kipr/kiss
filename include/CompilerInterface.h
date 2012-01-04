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

#ifndef __COMPILERINTERFACE_H__
#define __COMPILERINTERFACE_H__

#include <qplugin.h>
#include <QString>
#include <QStringList>
#include <QList>

#include "DebuggerInterface.h"
#include "Tab.h"
#include "Transaction.h"

#include <QStringList>

class CompilerInterface 
{
public:
	CompilerInterface() : m_error(false) {}

	virtual TransactionList* compile(const QString&, const QStringList&, bool source) { return 0; }
	
	const QStringList& getErrorMessages() const	{ return m_errorMessages; }
	const QStringList& getWarningMessages() const 	{ return m_warningMessages; }
	const QStringList& getLinkerMessages() const 	{ return m_linkerMessages; }
	const QStringList& getVerboseMessages() const 	{ return m_verboseMessages; }
	
	bool error() { return m_error; }
	const QString& errorTemplate() { return m_errorTemplate; } 
	const QStringList& errorArgs() { return m_errorArgs; } 
protected:
	void setError(bool error, const QString& errorTemplate, const QStringList& args)
	{
		m_error = error;
		m_errorTemplate = errorTemplate;
		m_errorArgs = args;
	}
	
	QStringList m_warningMessages, m_errorMessages, m_linkerMessages, m_verboseMessages;
private:
	bool m_error;
	QString m_errorTemplate;
	QStringList m_errorArgs;
};

Q_DECLARE_INTERFACE(TargetInterface, "com.kipr.kiss-ide.CompilerInterface/3.0");

#endif
