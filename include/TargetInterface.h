/**************************************************************************
 *  Copyright 2007,2008,2009 KISS Institute for Practical Robotics        *
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

#ifndef __TARGETINTERFACE_H__
#define __TARGETINTERFACE_H__

#include <qplugin.h>
#include <QString>
#include <QStringList>
#include <QList>
#include <QAction>
#include <QWidget>

#include "DebuggerInterface.h"
#include "Tab.h"

class TargetInterface 
{
public:
	// These methods will be called when their
	// respective editor buttons are activated
	virtual bool compile(const QString&, const QString&) { return false; }
	virtual bool run(const QString&, const QString&) { return false; }
	virtual void stop(const QString&) {}
	virtual bool download(const QString&, const QString&) { return false; }
	virtual bool simulate(const QString&, const QString&) { return false; }
	virtual DebuggerInterface* debug(const QString&, const QString&) { return 0; }
	virtual Tab* ui(const QString&) { return 0; }

	virtual bool hasDownload() 	{ return false; }
	virtual bool hasCompile() 	{ return false; }
	virtual bool hasRun() 		{ return false; }
	virtual bool hasStop() 		{ return false; }
	virtual bool hasSimulate() 	{ return false; }
	virtual bool hasDebug() 	{ return false; }
	virtual bool hasUi() 		{ return false; }
	
	const QList<QAction*>& 	getActionList() const 		{ return m_actionList; }
	const QStringList& 	getErrorMessages() const	{ return m_errorMessages; }
	const QStringList& 	getWarningMessages() const 	{ return m_warningMessages; }
	const QStringList& 	getLinkerMessages() const 	{ return m_linkerMessages; }
	const QStringList& 	getVerboseMessages() const 	{ return m_verboseMessages; }
	
	void setTargetFile(const QString& filename) { m_targetFile = filename; }
	
protected:
	QString m_targetFile;
	QStringList m_warningMessages, m_errorMessages, m_linkerMessages, m_verboseMessages;
	QList<QAction*> m_actionList;
};

Q_DECLARE_INTERFACE(TargetInterface, "com.kipr.kiss-c.TargetInterface/2.2");

#endif
