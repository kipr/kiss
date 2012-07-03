#ifndef _COMPUTER_H_
#define _COMPUTER_H_

#include "Filesystem.h"

#include <kiss-compiler/Compilation.h>
#include <QProcess>

class Computer
{
public:
	const bool run(const QString& name);
	CompilationPtr compile(const QString& name);
	const bool download(const QString& name, TinyArchive *archive);
	Filesystem *filesystem();
	
private:
	Filesystem m_filesystem;
	QMap<QString, QStringList> m_compileResults;
	QProcess m_outputBinary;
};

#endif
