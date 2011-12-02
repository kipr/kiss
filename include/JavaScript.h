#ifndef _JAVASCRIPT_H_
#define _JAVASCRIPT_H_

#include <QFile>

#include <QScriptEngine>
#include <QScriptValue>
#include <QScriptValueIterator>

class JavaScript;

struct JavaScriptEnvironment
{
	virtual void prepare(JavaScript* engine) = 0;
};

class JavaScript
{
public:
	JavaScript(JavaScriptEnvironment* env);
	
	QScriptValue evaluate(QFile& file);
	QScriptValue evaluate(const QString& str);
	
	QScriptEngine* engine();
	
	static QMap<QString, QScriptValue> objectMap(QScriptValue object);
	static QMap<QString, QString> objectStringMap(QScriptValue object);
private:
	QScriptEngine m_engine;
	JavaScriptEnvironment* m_env;
};

#endif
