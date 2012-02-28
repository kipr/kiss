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

#ifndef __TARGETINTERFACE_H__
#define __TARGETINTERFACE_H__

#include <qplugin.h>
#include <QString>
#include <QStringList>
#include <QList>
#include <QAction>
#include <QWidget>
#include <QByteArray>

#include "DebuggerInterface.h"
#include "Tab.h"

#include <QStringList>

class TargetInterface 
{
public:
	TargetInterface() : m_error(false) {}
	
	// These methods will be called when their
	// respective editor buttons are activated
	virtual bool compile(const QString&, const QString&) { return false; }
	virtual bool run(const QString&, const QString&) { return false; }
	virtual void stop(const QString&) {}
	virtual int download(const QString&, const QString&) { return TargetInterface::UnknownError; }
	virtual bool rawDownload(const QString&, const QString&) { return false; }
	virtual bool simulate(const QString&, const QString&) { return false; }
	virtual bool debugConsole(const QString&, const QString&, const QList<Location>&) { return false; }
	virtual DebuggerInterface* debug(const QString&, const QString&) { return 0; }
	virtual TabbedWidget* ui(const QString&) { return 0; }
	
	virtual bool hasScreenGrab() { return false; }
	virtual QByteArray screenGrab(const QString&) { return QByteArray(); }
	
	virtual bool hasFileRequest() 	{ return false; }
	virtual QStringList requestDir(const QString&, const QString&) { return QStringList(); }
	virtual QByteArray requestFile(const QString&, const QString&) { return QByteArray(); }

	virtual bool hasDownload() 	{ return false; }
	virtual bool hasRawDownload() 	{ return false; }
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
	
	bool error() { return m_error; }
	const QString& errorTemplate() { return m_errorTemplate; } 
	const QStringList& errorArgs() { return m_errorArgs; } 
	
	void setTargetFile(const QString& filename) { m_targetFile = filename; }
	
	template<typename T>
	void free(T* ptr) { delete ptr; }
	
	enum {
		NoError = 0,
		CompileFailed,
		DownloadFailed,
		UnknownError
	};
protected:
	void setError(bool error, const QString& errorTemplate, const QStringList& args)
	{
		m_error = error;
		m_errorTemplate = errorTemplate;
		m_errorArgs = args;
	}
	
	//! Simplifies includes by providing a proxy for plugins to create errors with
	void errorDialogProxy(QWidget* parent, const QString& error, const QStringList& args = QStringList());
	
	QString m_targetFile;
	QStringList m_warningMessages, m_errorMessages, m_linkerMessages, m_verboseMessages;
	QList<QAction*> m_actionList;
private:
	bool m_error;
	QString m_errorTemplate;
	QStringList m_errorArgs;
};

Q_DECLARE_INTERFACE(TargetInterface, "com.kipr.kiss-c.TargetInterface/3.1");

#endif
