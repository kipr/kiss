#include "JavaScript.h"

#include <QTextStream>

JavaScript::JavaScript(JavaScriptEnvironment* env) : m_env(env) { env->prepare(this); }

QScriptValue JavaScript::evaluate(QFile& file)
{
	if(!file.open(QIODevice::ReadOnly)) return 0;
	QString str = QTextStream(&file).readAll();
	file.close();
	return evaluate(str);
}

QScriptValue JavaScript::evaluate(const QString& str)
{
	QScriptValue ret = m_engine.evaluate(str);
	QScriptValue exception = m_engine.uncaughtException();
	return exception.isValid() ? exception : QScriptValue();
}

QScriptEngine* JavaScript::engine() { return &m_engine; }

QMap<QString, QScriptValue> JavaScript::objectMap(QScriptValue object)
{
	QMap<QString, QScriptValue> ret;
	QScriptValueIterator it(object);
	while(it.hasNext()) {
		it.next();
		ret[it.name()] = it.value();
	}
	return ret;
}

QMap<QString, QString> JavaScript::objectStringMap(QScriptValue object)
{
	QMap<QString, QString> ret;
	QScriptValueIterator it(object);
	while(it.hasNext()) {
		it.next();
		ret[it.name()] = it.value().toString();
	}
	return ret;
}