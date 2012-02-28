#include "Compiler.h"

#include <QVariant>
#include <QScriptEngine>

CompileResult::CompileResult(bool success, const QStringList& verboseMessages, const QStringList& errorMessages,
	const QStringList& warningMessages, const QStringList& linkerMessages)
	: success(success), verboseMessages(verboseMessages), errorMessages(errorMessages),
	warningMessages(warningMessages), linkerMessages(linkerMessages) {}

Compiler::Compiler(const QString& name, const QStringList& types) : m_name(name), m_types(types)
{
	
}

const QString& Compiler::name() const
{
	return m_name;
}

const QStringList& Compiler::types() const
{
	return m_types;
}

CompilerPlugin::CompilerPlugin(const QScriptValue& plugin)
	: Compiler(plugin.property("name").toString(), plugin.property("types").toVariant().toStringList()), m_plugin(plugin)
{
}

CompileResult CompilerPlugin::compile(const QStringList& files)
{
	QScriptValue ret = m_plugin.property("compile").call(QScriptValue(), QScriptValueList() << m_plugin.engine()->toScriptValue(files));
	
	return CompileResult(ret.property("success").toBool(), ret.property("verboseMessages").toVariant().toStringList(),
		ret.property("errorMessages").toVariant().toStringList(), ret.property("warningMessages").toVariant().toStringList(),
		ret.property("linkerMessages").toVariant().toStringList());
}