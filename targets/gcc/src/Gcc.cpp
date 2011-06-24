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
 
#include "Gcc.h"

#include <QProcess>
#include <QString>
#include <QFileInfo>
#include <QDir>
#include <QRegExp>
#include <QtGlobal>
#include <QBuffer>
#include <QApplication>
#include <QMessageBox>
#include <QDateTime>

#include "Gdb.h"

Gcc::Gcc()
{
#ifdef Q_OS_WIN32
	m_gccPath = QDir::currentPath() + "/targets/gcc/mingw/bin/g++.exe";
#elif defined(Q_OS_MAC)
  m_gccPath="/usr/bin/g++";
#else
	m_gccPath="/usr/bin/g++";
#endif

	QFileInfo gccExecutable(m_gccPath);
	if(!gccExecutable.exists())
		QMessageBox::critical(0, "Error", "Could not find GCC Executable!");

	m_gcc.setReadChannel(QProcess::StandardError);

//FIXME This is ugly
#ifdef Q_OS_MAC
	system(("ranlib " + QDir::currentPath() + "/targets/gcc/lib/*.a").toLocal8Bit());
#endif

	// m_actionList.push_back(m_toolbar.toolbarAction());
}

Gcc::~Gcc()
{
	m_gcc.kill();
	m_outputBinary.kill();
}

bool Gcc::compile(const QString& filename, const QString& port)
{
	return compile(filename, port, false);
}

bool Gcc::run(const QString& filename, const QString& port)
{
	if(!compile(filename, port))
		return false;

	QString outputString;
	QFileInfo outputFileInfo(m_outputFileName);
	QFile scriptFile;

#ifdef Q_OS_WIN32
	scriptFile.setFileName(QDir::temp().absoluteFilePath("kiprBatchFile.cmd"));
	outputString += "@echo off\n";
	outputString += "\"" + QDir::toNativeSeparators(outputFileInfo.absolutePath()) + "\\" + outputFileInfo.fileName() + "\"\n";
	outputString +=  "pause\n";
#else
	scriptFile.setFileName(QDir::temp().absoluteFilePath("kiprScript.sh"));
	outputString += "#!/bin/bash\n";
	outputString += "cd \"" + outputFileInfo.absolutePath() + "\"\n";
	outputString += "\"./" + outputFileInfo.fileName() + "\"\n";
#endif

	if(!scriptFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
		qWarning("Gcc::run() Unable to open temp file for writing");
		return false;
	}

	scriptFile.setPermissions(scriptFile.permissions() | QFile::ExeOwner);
	scriptFile.write(outputString.toLocal8Bit());
	scriptFile.close();

	QStringList args;
	QFileInfo scriptInfo(scriptFile);

	m_outputBinary.setWorkingDirectory(outputFileInfo.absolutePath());

#ifdef Q_OS_WIN32
	QString startLine = "start \"" + m_outputFileName + "\" \"cmd /c " +
						scriptInfo.absoluteFilePath() + "\"\n";
	args << "/k";
	m_outputBinary.start("cmd", args);
	m_outputBinary.write(startLine.toLocal8Bit());
	m_outputBinary.write("exit\n");
#elif defined(Q_OS_MAC)
	args << "-a" << "/Applications/Utilities/Terminal.app" << scriptInfo.absoluteFilePath();
	m_outputBinary.start("open", args);
#else
	args << "-e" << scriptInfo.absoluteFilePath() + " && echo \"\nQuitting in 5 secs...\" && sleep 5";
	m_outputBinary.start("xterm", args);
#endif

	return true;

}

DebuggerInterface* Gcc::debug(const QString& filename, const QString& port)
{
	if(!compile(filename, port, true)) 
		return 0;
		
	return new Gdb(m_outputFileName);
}

void Gcc::processCompilerOutput()
{
	bool foundError=false,foundWarning=false;
	m_errorMessages.clear();
	m_warningMessages.clear();
	m_verboseMessages.clear();

	while(m_gcc.canReadLine()) {
		QString inputLine = QString::fromLocal8Bit(m_gcc.readLine());
		QString outputLine;

		inputLine.remove(QRegExp("\\r*\\n$"));

		m_verboseMessages << inputLine;

		inputLine.remove(QRegExp("^C\\:"));
		inputLine.remove(QRegExp("^/.*/(?=\\S*\\:)"));

		outputLine += inputLine.section(':',0,0) + ":";

		if(inputLine.section(':',2,2).length() > 0) {
			outputLine += inputLine.section(':',1,1) + ":";
			outputLine += inputLine.section(':',2,2).remove(' ') + ":";
			outputLine += inputLine.section(':', 3);
		}
		else
			outputLine += inputLine.section(':', 1);

		if(outputLine.section(':',2,2) == "error") {
			m_errorMessages << outputLine;
			foundError=true;
		}
		else if(outputLine.section(':', 2,2) == "warning") {
			m_warningMessages << outputLine;
			foundWarning=true;
		}
		else {
			m_errorMessages << inputLine;
			m_warningMessages << inputLine;
		}
	}
	if(!foundError && !foundWarning)
		m_warningMessages.clear();
	else if(!foundError)
		m_errorMessages.clear();
	else if(!foundWarning)
		m_warningMessages.clear();
}

void Gcc::processLinkerOutput()
{
	m_linkerMessages.clear();
	while(m_gcc.canReadLine()) {
		QString line = QString::fromLocal8Bit(m_gcc.readLine()).remove(QRegExp("\\r*\\n$"));
		m_verboseMessages << line;
		m_linkerMessages << line;
	}
}

void Gcc::refreshSettings()
{
	QStringList include_dirs;
	QStringList lib_dirs;
	QSettings settings(m_targetFile, QSettings::IniFormat);

	m_cflags.clear();
	m_lflags.clear();

	include_dirs = settings.value("Target/include_dirs").toString().split(' ', QString::SkipEmptyParts);
	lib_dirs = settings.value("Target/lib_dirs").toString().split(' ', QString::SkipEmptyParts);

	QStringListIterator i(include_dirs);
	while(i.hasNext()) {
		QDir includePath(i.next());
		if(includePath.isAbsolute())
			m_cflags << "-I" + includePath.path();
		else
			m_cflags << "-I" + QDir::currentPath() + "/" + includePath.path();
	}

	i = lib_dirs;
	while(i.hasNext()) {
		QDir libPath(i.next());
		if(libPath.isAbsolute())
			m_lflags << "-L" + libPath.path();
		else
			m_lflags << "-L" + QDir::currentPath() + "/" + libPath.path();
	}

	m_cflags << settings.value("Target/cflags").toString().split(' ', QString::SkipEmptyParts);
	m_lflags << settings.value("Target/lflags").toString().split(' ', QString::SkipEmptyParts);
	
#ifdef Q_OS_MAC
	if(QSysInfo::MacintoshVersion == QSysInfo::MV_TIGER) {
    m_cflags << "-isysroot" << "/Developer/SDKs/MacOSX10.4u.sdk";
    m_lflags << "-isysroot" << "/Developer/SDKs/MacOSX10.4u.sdk";
	}
#endif
}

bool Gcc::compile(const QString& filename, const QString& port, bool debug)
{
	QString p = port;
	
	QFileInfo sourceInfo(filename);
	QStringList args;

	refreshSettings();

#ifdef Q_OS_WIN32
	m_outputFileName = sourceInfo.dir().absoluteFilePath(sourceInfo.baseName() + ".exe");
#else
	m_outputFileName = sourceInfo.dir().absoluteFilePath(sourceInfo.baseName());
#endif
	QString objectName = sourceInfo.dir().absoluteFilePath(sourceInfo.baseName() + ".o");

	QFileInfo outputInfo(m_outputFileName);
	if(sourceInfo.lastModified() < outputInfo.lastModified())
		return true;

	args = m_cflags;
	p.replace("\\", "\\\\");
	args << "-DDEFAULT_SERIAL_PORT=\"" + p + "\"";
	args << "-c" << filename << "-o" << objectName;
	if(debug) args << "-g" << "-pg";
	m_gcc.start(m_gccPath, args);
	m_gcc.waitForFinished();
	processCompilerOutput();
	m_linkerMessages.clear();

	if(m_gcc.exitCode() != 0)
		return false;

	args.clear();
	args << "-o" << m_outputFileName << objectName;
	args << m_lflags;
	m_gcc.start(m_gccPath, args);
	m_gcc.waitForFinished();
	processLinkerOutput();

	QFile objectFile(objectName);
	if(!debug) objectFile.remove();

	if(m_gcc.exitCode() == 0)
		return true;
	return false;
}

Q_EXPORT_PLUGIN2(gcc_plugin, Gcc);
