#include "Kovan.h"

#include <QDebug>

#include <kiss-compiler/ArchiveWriter.h>
#include <kiss-compiler/Temporary.h>
#include <kiss-compiler/CompilerManager.h>

const bool Kovan::run(const QString& name)
{
	if(!m_filesystem.program(name)) return false;
	qDebug() << name << "has the following results avail for running:" << m_compileResults.value(name);
	if(!m_compileResults.value(name).size()) {
		qCritical() << "Cannot run" << name << ". No results.";
		return false;
	}
	
	m_outputBinary.kill();

	QFileInfo outputFileInfo(m_compileResults.value(name)[0]);

	QString outputString;

	QFile scriptFile;

#ifdef Q_OS_WIN32
	scriptFile.setFileName(QDir::temp().absoluteFilePath("kiprBatchFile.cmd"));
	outputString += "@echo off\r\n";
	outputString += "set PATH=%PATH%;" + QDir::toNativeSeparators(QDir::currentPath()) + "\\targets\\gcc\\mingw\\bin\r\n";
	outputString += "\"" + QDir::toNativeSeparators(outputFileInfo.absolutePath()) + "\\" + outputFileInfo.fileName() + "\"\r\n";
	outputString +=  "pause\r\n";
#else
	scriptFile.setFileName(QDir::temp().absoluteFilePath("kiprScript.sh"));
	outputString += "#!/bin/bash\n";
	outputString += "cd \"" + outputFileInfo.absolutePath() + "\"\n";
	outputString += "chmod +x \"" + outputFileInfo.fileName() + "\"\n";
	outputString += "clear\n";
	outputString += "\"./" + outputFileInfo.fileName() + "\"\n";
#endif

	if(!scriptFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
		qCritical() << "ThisMachine::run() Unable to open temp file for writing";
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

CompilationPtr Kovan::compile(const QString& name)
{
	TinyArchive *archive = m_filesystem.program(name);
	if(!archive) return CompilationPtr();
	
	ArchiveWriter writer(archive, Temporary::subdir(name));
	QMap<QString, QString> settings;
	QByteArray rawSettings = QTinyNode::data(archive->lookup("settings:"));
	QDataStream stream(rawSettings);
	stream >> settings;
	
	CompilationPtr compilation(new Compilation(CompilerManager::ref().compilers(), name, writer.files(), settings, "kovan"));
	bool success = compilation->start();
	qDebug() << "Results:" << compilation->compileResults();
	
	qDebug() << (success ? "Compile Succeeded" : "Compile Failed");
	
	if(success) m_compileResults[name] = compilation->compileResults();
	else m_compileResults.remove(name);
	
	return compilation;
}

const bool Kovan::download(const QString& name, TinyArchive *archive)
{
	m_filesystem.setProgram(name, archive);
	m_compileResults.remove(name);
	return true;
}

Filesystem *Kovan::filesystem()
{
	return &m_filesystem;
}