#ifndef _COMPILERCPP_H_
#define _COMPILERCPP_H_

#include <QtPlugin>
#include "Compiler.h"

class QProcessSegment;

class CompilerCpp : public QObject, public Compiler
{
Q_OBJECT
Q_INTERFACES(Compiler)
public:
	CompilerCpp();
	
	virtual CompileResult compile(Compilation* compilation, const QStringList& files);
private:
	QProcessSegment* createGccSegment(const QStringList& args);
	static QString gccPath();
};

#endif
