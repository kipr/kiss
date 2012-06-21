#include "CompilerO.h"

#include "CommandChain.h"
#include "Compilation.h"
#include "Temporary.h"
#include "GccOutput.h"

#include <QFileInfo>
#include <QDebug>

CompilerO::CompilerO()
	: Compiler("ld", QStringList() << "o")
{
	
}

CompileResult CompilerO::compile(Compilation* compilation, const QStringList& files)
{
	int idealProcesses = QThread::idealThreadCount();
	CommandChain chain(idealProcesses > 0 ? idealProcesses : 1);
	QStringList cFlags = compilation->settings()["LD_FLAGS"].split(" ");
	const QString& executable = compilation->name();
	QStringList strippedFiles;
	foreach(const QString& file, files) {
		QFileInfo fi(file);
		strippedFiles << fi.path() + "/" + fi.fileName();
	}
	
	chain.add(createGccSegment(QStringList() << "-o" << executable << files));
	qDebug() << "Creating executable" << executable;
	bool success = chain.execute();
	QIODevice* out = chain.chainSession()->out();
	QIODevice* err = chain.chainSession()->err();
	out->seek(0);
	err->seek(0);
	if(!success) {
		qWarning() << "Chain execution failed";
	} else compilation->addCompileResult(outputDirectory().path() + "/" + executable);
	
	return CompileResult(success) + GccOutput::processLinkerOutput(err);
}

QProcessSegment* CompilerO::createGccSegment(const QStringList& args)
{
	QProcessSegment* ret = new QProcessSegment(gccPath(), args);
	ret->process()->setWorkingDirectory(outputDirectory().path());
#ifdef Q_OS_WIN32
	QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
	env.insert("PATH", env.value("Path") + ";" + QDir::toNativeSeparators(QDir::currentPath() + "/targets/gcc/mingw/bin"));
	ret->process()->setProcessEnvironment(env);
#endif
	return ret;
}

QString CompilerO::gccPath()
{
#ifdef Q_OS_WIN32
	return QDir::currentPath() + "/targets/gcc/mingw/bin/gcc.exe";
#else
	return "/usr/bin/gcc";
#endif
}

Q_EXPORT_PLUGIN2(o_compiler, CompilerO);