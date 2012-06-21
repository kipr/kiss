#include "GccOutput.h"

#include "ErrorWidget.h"

#include <QIODevice>

#define LINKER_KEY "linker"

CompileResult GccOutput::processLinkerOutput(QIODevice* in)
{
	QStringList verboseMessages;
	QStringList linkerMessages;
	
	while(in->canReadLine()) {
		QString line = QString::fromLocal8Bit(in->readLine()).remove(QRegExp("\\r*\\n$"));
		verboseMessages << line;
		linkerMessages << line;
	}
	
	QMap<QString, QStringList> ret;
	if(linkerMessages.size()) ret[DEFAULT_LINKER_KEY] = linkerMessages;
	
	return CompileResult(true, ret, verboseMessages.join("\n"));
}

