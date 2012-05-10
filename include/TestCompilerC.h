#ifndef _TESTCOMPILERC_H_
#define _TESTCOMPILERC_H_

#include "Compiler.h"

class QProcessSegment;

class TestCompilerC : public Compiler
{
public:
	TestCompilerC();
	
	virtual CompileResult compile(Compilation* compilation, const QStringList& files);
private:
	QProcessSegment* createGccSegment(const QStringList& args);
	static QString gccPath();
};

#endif
