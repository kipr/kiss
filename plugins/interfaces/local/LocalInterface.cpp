#include "LocalInterface.h"

#include <kiss-compiler/Temporary.h>
#include "ArchiveWriter.h"

#include <QProcess>
#include <QDir>
#include <QTextStream>
#include <QDebug>

class ThisMachine : public Device
{
public:
	ThisMachine(Interface* interface) : Device(interface, "This Machine") {}
	
	virtual const QMap<QString, QString> information() const {
		QMap<QString, QString> ret;
		ret[DISPLAY_NAME] = QString("This Machine");
	#ifdef Q_OS_WIN
		ret[DEVICE_TYPE] = "Windows Computer";
	#elif defined(Q_OS_MAC)
		ret[DEVICE_TYPE] = "Mac OS X Computer";
	#else
		ret[DEVICE_TYPE] = "*nix Computer";
	#endif
		ret[DOWNLOAD_TYPE] = DOWNLOAD_BINARY;
		ret[SERIAL] = NOT_AVAILABLE;
		return ret;
	}
	
	virtual const bool available()
	{
		return true;
	}
	
	virtual const CompileResult compile(const QString& name)
	{
		return CompileResult(false);
	}
	
	virtual const bool download(const QString& name, TinyArchive* archive)
	{
		QDir namedDir = dirFor(name);
		if(!namedDir.exists()) QDir().mkpath(namedDir.path());
		ArchiveWriter writer(archive, namedDir);
		return writer.write();
	}
	
	virtual const bool run(const QString& name)
	{
		m_outputBinary.kill();
		QDir namedDir = dirFor(name);
		if(!namedDir.exists()) {
			qCritical() << "This Machine run failed." << namedDir.path() << "not found";
			return false;
		}
		
		QFile run(namedDir.path() + "/run");
		run.open(QIODevice::ReadOnly);
		QTextStream stream(&run);
		QFileInfo outputFileInfo(namedDir.path() + "/" + stream.readAll());
		run.close();
		
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
	
private:
	static QDir dirFor(const QString& name)
	{
		QDir this_machine = Temporary::subdir("this_machine");
		QDir namedDir = this_machine.path() + "/" + name;
		return namedDir;
	}
	
	QProcess m_outputBinary;
};

LocalInterface::LocalInterface() : Interface("Local") {}

QList<DevicePtr> LocalInterface::devices()
{
	return QList<DevicePtr>() << DevicePtr(new ThisMachine(this));
}

const bool LocalInterface::local() { return true; }

Q_EXPORT_PLUGIN2(local_interface, LocalInterface);