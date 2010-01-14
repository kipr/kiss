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

#include "LexerSpec.h"
#include "LexerStyles.h"
#include "SourceFile.h"

class TargetInterface 
{
public:
	// Destructor
	virtual ~TargetInterface() {}

	// These methods will be called when their
	// respective editor buttons are activated
	virtual bool compile(QString filename) = 0;	
	virtual bool run(QString filename) = 0;
	virtual void stop() = 0;
	virtual bool download(QString filename) = 0;
	virtual bool simulate(QString filename) = 0;

	// These should inform the plugin loader of
	// The features offered by this plugin
	virtual bool hasDownload() = 0;
	virtual bool hasCompile() = 0;
	virtual bool hasRun() = 0;
	virtual bool hasStop() = 0;
	virtual bool hasSimulate() = 0;
	
	/* Builtin Stuff */
	QList<QAction*> getActionList() {return m_actionList;}
	QStringList getErrorMessages() {return m_errorMessages;}
	QStringList getWarningMessages() {return m_warningMessages;}
	QStringList getLinkerMessages() {return m_linkerMessages;}
	QStringList	getVerboseMessages() {return m_verboseMessages;}
	LexerSpec&	getLexerSpec() {return m_lexerSpec;}
	QString		getPort() {return m_defaultPort;}
	
	void setPort(QString port) {m_defaultPort=port;}
	void setTargetFile(QString filename) {m_targetFile=filename;}
	
	virtual void setCurrentFile(SourceFile *sourceFile) = 0;
	
	/* Also, you have the option of defining a signal
		named "void requestPort()" in your plugin.  
		This signal will pop up a port selection dialog
		when emitted.  The signal itself cannot be defined
		here because this interface is not a QObject
signal:
	void requestPort();								 */
	
protected:
	LexerSpec m_lexerSpec;
	QString m_defaultPort;
	QString m_targetFile;
	QStringList m_warningMessages, m_errorMessages, 
				m_linkerMessages, m_verboseMessages;
	QList<QAction*> m_actionList;
};

Q_DECLARE_INTERFACE(TargetInterface, "com.kipr.kiss-c.TargetInterface/2.1");

#endif
