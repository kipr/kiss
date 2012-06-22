#ifndef _GCCOUTPUT_H_
#define _GCCOUTPUT_H_

#include "Compiler.h"

#include <QMap>
#include <QString>
#include <QStringList>

class QIODevice;

class GccOutput
{
public:
	static CompileResult processCompilerOutput(QIODevice* in);
};

#endif
