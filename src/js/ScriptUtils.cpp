#include "ScriptUtils.h"

#include <QScriptValueIterator>

QMap<QString, QScriptValue> ScriptUtils::objectMap(QScriptValue object)
{
	QMap<QString, QScriptValue> ret;
	QScriptValueIterator it(object);
	while(it.hasNext()) {
		it.next();
		ret[it.name()] = it.value();
	}
	return ret;
}

QMap<QString, QString> ScriptUtils::objectStringMap(QScriptValue object)
{
	QMap<QString, QString> ret;
	QScriptValueIterator it(object);
	while(it.hasNext()) {
		it.next();
		ret[it.name()] = it.value().toString();
	}
	return ret;
}