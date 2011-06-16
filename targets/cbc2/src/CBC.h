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

#ifndef __CBC_H__
#define __CBC_H__

#include <qplugin.h>
#include <QProcess>
#include <QString>
#include <QSettings>

#include "TargetInterface.h"
#include "LexerSpec.h"
#include "LexerStyles.h"
#include "SerialClient.h"

class CBC : public QObject, public TargetInterface
{
	Q_OBJECT
	Q_INTERFACES(TargetInterface)
	
public:
	CBC();
	~CBC();

	bool compile(QString filename, QString port);
	bool run(QString,QString) { return false; }
	void stop(QString) {}
	bool download(QString filename, QString port);
	bool simulate(QString filename, QString port);

	bool hasCompile() { return true; }
	bool hasRun() { return false; }
	bool hasDownload() { return true; }
	bool hasStop() { return false; }
	bool hasSimulate() { return true; }

private:
	QProcess m_gcc;
	QProcess m_outputBinary;
	QString m_gccPath;
	QString m_outputFileName;
	QStringList m_cflags,m_lflags;
  	SerialClient m_serial;
	
	void processCompilerOutput();
	void processLinkerOutput();
	void refreshSettings();
	void setLexerSpecs();
	
  	QStringList getPaths(QString string);
};

#endif
