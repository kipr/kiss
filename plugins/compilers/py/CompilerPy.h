#ifndef _COMPILERPY_H_
#define _COMPILERPY_H_

#include <QtPlugin>
#include "Compiler.h"

class QProcessSegment;

class CompilerPy : public QObject, public Compiler
{
Q_OBJECT
Q_INTERFACES(Compiler)
public:
	CompilerPy();
	
	virtual CompileResult compile(Compilation* compilation, const QStringList& files);
private:
	QProcessSegment* createPythonSegment(const QStringList& args);
	static QString pythonPath();
};

#endif
