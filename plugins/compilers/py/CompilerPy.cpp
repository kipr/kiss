#include "CompilerPy.h"

#include "CommandChain.h"
#include "Compilation.h"
#include "Temporary.h"

#include "GccOutput.h"

#include <QFileInfo>
#include <QDebug>

CompilerPy::CompilerPy()
	: Compiler("python", QStringList() << "py")
{
	
}

CompileResult CompilerPy::compile(Compilation* compilation, const QStringList& files)
{
	int idealProcesses = QThread::idealThreadCount();
	CommandChain chain(idealProcesses > 0 ? idealProcesses : 1);
	QStringList pyFlags = compilation->settings()["PY_FLAGS"].split(" ");
	foreach(const QString& file, files) {
		QFileInfo fi(file);
		QString output = fi.path() + "/" + fi.baseName();
		output = output.replace("/", "_");
		output += ".o";
		chain.add(createGccSegment(QStringList(cFlags) << "-c" << file << "-o" << output));
		compilation->addFile(outputDirectory().path() + "/" + output);
	}
	bool success = chain.execute();
	QIODevice* out = chain.chainSession()->out();
	QIODevice* err = chain.chainSession()->err();
	out->seek(0);
	err->seek(0);
	if(!success) {
		qWarning() << "Chain execution failed";
	} else qWarning() << "Chain execution succeeded";
	return CompileResult(success) + GccOutput::processCompilerOutput(err);
}

QProcessSegment* CompilerPy::createPythonSegment(const QStringList& args)
{
	QProcessSegment* ret = new QProcessSegment(pythonPath(), args);
	ret->process()->setWorkingDirectory(outputDirectory().path());
	return ret;
}

QString CompilerPy::pythonPath()
{
#ifdef Q_OS_WIN32
	return "python"; // TODO: Windows
#else
	return "/usr/bin/python";
#endif
}

Q_EXPORT_PLUGIN2(py_compiler, CompilerPy);