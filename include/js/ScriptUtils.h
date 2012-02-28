#ifndef _SCRIPTUTILS_H_
#define _SCRIPTUTILS_H_

#include <QScriptValue>
#include <QMap>
#include <QString>

class ScriptUtils
{
public:
	static QMap<QString, QScriptValue> objectMap(QScriptValue object);
	static QMap<QString, QString> objectStringMap(QScriptValue object);
};

#endif
