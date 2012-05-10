#include "TestCompilerC.h"

#include "CommandChain.h"
#include "Compilation.h"
#include "Temporary.h"

#include "GccOutput.h"

#include <QFileInfo>
#include <QDebug>

TestCompilerC::TestCompilerC()
	: Compiler("gcc", QStringList() << "c")
{
	
}

CompileResult TestCompilerC::compile(Compilation* compilation, const QStringList& files)
{
	int idealProcesses = QThread::idealThreadCount();
	CommandChain chain(idealProcesses > 0 ? idealProcesses : 1);
	QStringList cFlags = compilation->settings()["C_FLAGS"].split(" ");
	foreach(const QString& file, files) {
		QFileInfo fi(file);
		QString output = fi.path() + "/" + fi.baseName();
		output = output.replace("/", "_");
		output += ".o";
		chain.add(createGccSegment(QStringList(cFlags) << "-c" << file << "-o" << output));
		compilation->addFile(outputDirectory().path() + "/" + output);
	}
	bool success = chain.execute();
	if(!success) {
		qWarning() << "Chain execution failed";
	} else qWarning() << "Chain execution succeeded";
	return CompileResult(success) + GccOutput::processCompilerOutput(chain.chainSession()->err());
}

QProcessSegment* TestCompilerC::createGccSegment(const QStringList& args)
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

QString TestCompilerC::gccPath()
{
#ifdef Q_OS_WIN32
	return QDir::currentPath() + "/targets/gcc/mingw/bin/gcc.exe";
#else
	return "/usr/bin/gcc";
#endif
}