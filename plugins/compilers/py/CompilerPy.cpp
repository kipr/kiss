#include "CompilerPy.h"

#include "CommandChain.h"
#include "Compilation.h"
#include "Temporary.h"

#include "ErrorWidget.h"

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
		chain.add(createPythonSegment(QStringList() << "-m" << "py_compile" << file));
	}
	bool success = chain.execute();
	QIODevice* out = chain.chainSession()->out();
	QIODevice* err = chain.chainSession()->err();
	err->putChar('\n');
	out->seek(0);
	err->seek(0);
	if(!success) {
		qWarning() << "Chain execution failed";
	} else qWarning() << "Chain execution succeeded";
	return CompileResult(success) + processCompile(out) + processCompile(err);
}

QString CompilerPy::extractFirstString(const QString& str)
{
	int start = str.indexOf('\'');
	if(start < 0) return "";
	
	int end = str.indexOf('\'', start + 1);
	if(end < 0) return str;
	
	return str.mid(start + 1, end - start - 1);
}

CompileResult CompilerPy::processCompile(QIODevice *err)
{
	QStringList problems;
	QStringList verbose;
	qDebug() << "err length" << err->bytesAvailable();
	while(err->canReadLine()) {
		QString line = err->readLine();
		qDebug() << "Processing line" << line;
		line = line.trimmed();
		if(line.isEmpty()) continue;
		verbose << line;
		int start = line.indexOf('(');
		if(start < 0) {
			problems << line;
			continue;
		}
		int info = line.indexOf('(', start + 1);
		if(info < 0) {
			problems << line;
			continue;
		}
		
		QString error = line.left(start);
		QString message = extractFirstString(line.mid(start));
		QString file = extractFirstString(line.mid(info));
		file.remove(QRegExp("^/(.*/)*"));
		QString rest = line.mid(info);
		QString lineNumber = rest.section(',', 1, 1);
		QString product = file.trimmed() + ":" + lineNumber.trimmed() + ":" + error.remove("Sorry:").trimmed() + " " + message;
		qDebug() << product;
		problems << product;
	}
	QMap<QString, QStringList> messages;
	if(problems.size()) messages[DEFAULT_ERROR_KEY] = problems;
	return CompileResult(true, messages, verbose.join("\n"));
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
	return "python";
#endif
}

Q_EXPORT_PLUGIN2(py_compiler, CompilerPy);