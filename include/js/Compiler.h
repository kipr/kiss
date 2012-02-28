#ifndef _COMPILER_H_
#define _COMPILER_H_

#include <QString>
#include <QStringList>
#include <QScriptValue>

struct CompileResult
{
	CompileResult(bool success, const QStringList& verboseMessages, const QStringList& errorMessages,
		const QStringList& warningMessages, const QStringList& linkerMessages);
	
	const bool success;
	
	const QStringList verboseMessages;
	const QStringList errorMessages;
	const QStringList warningMessages;
	const QStringList linkerMessages;
};

class Compiler
{
public:
	Compiler(const QString& name, const QStringList& types);
	
	const QString& name() const;
	const QStringList& types() const;
	
	virtual CompileResult compile(const QStringList& files) = 0;
private:
	QString m_name;
	QStringList m_types;
};

class CompilerPlugin : public Compiler
{
public:
	CompilerPlugin(const QScriptValue& plugin);

	virtual CompileResult compile(const QStringList& files);
	
private:
	QScriptValue m_plugin;
};

#endif
