#ifndef _COMPILERC_H_
#define _COMPILERC_H_

#include <QtPlugin>
#include "Compiler.h"

class QProcessSegment;

class CompilerC : public QObject, public Compiler
{
Q_OBJECT
Q_INTERFACES(Compiler)
public:
	CompilerC();
	
	virtual CompileResult compile(Compilation* compilation, const QStringList& files);
private:
	QProcessSegment* createGccSegment(const QStringList& args);
	static QString gccPath();
};

#endif
