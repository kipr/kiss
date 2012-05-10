#ifndef _TESTCOMPILERO_H_
#define _TESTCOMPILERO_H_

#include "Compiler.h"

class QProcessSegment;

class TestCompilerO : public Compiler
{
public:
	TestCompilerO();
	
	virtual CompileResult compile(Compilation* compilation, const QStringList& files);
private:
	QProcessSegment* createGccSegment(const QStringList& args);
	static QString gccPath();
};

#endif
