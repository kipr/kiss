#ifndef _COMPILERO_H_
#define _COMPILERO_H_

#include <QtPlugin>
#include "Compiler.h"

class QProcessSegment;

class CompilerO : public QObject, public Compiler
{
Q_OBJECT
Q_INTERFACES(Compiler)
public:
	CompilerO();
	
	virtual CompileResult compile(Compilation* compilation, const QStringList& files);
private:
	QProcessSegment* createGccSegment(const QStringList& args);
	static QString gccPath();
};

#endif
