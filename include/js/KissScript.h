#ifndef _KISSSCRIPT_H_
#define _KISSSCRIPT_H_

#include <QObject>
#include <QScriptable>
#include <QScriptValue>

class KissScript : public QObject, public QScriptable
{
Q_OBJECT
Q_PROPERTY(QString os READ os)
Q_PROPERTY(QString cwd READ cwd)
public:
	KissScript();
	
	QString os() const;
	QString cwd() const;
	
public slots:
	QScriptValue plugin(const QString& type, const QScriptValue& obj);
	void log(const QString& from, const QString& message);
	void log(const QString& message);
	
	QScriptValue process();
	
private:
	
};

#endif
