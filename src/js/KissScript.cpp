#include "KissScript.h"

#include "Compilers.h"
#include "Compiler.h"

#include "Process.h"

#include "Kiss.h"

#include <iostream>
#include <QScriptEngine>

#include <QDebug>
#include <QDir>

KissScript::KissScript()
{
	
}

QString KissScript::os() const
{
	return OS_NAME;
}

QString KissScript::cwd() const
{
	return QDir::currentPath();
}

QScriptValue KissScript::plugin(const QString& type, const QScriptValue& obj)
{
	if(type == "compiler") {
		Compilers::ref().addCompiler(new CompilerPlugin(obj));
		return true;
	}
	if(type == "lexer") {
		qWarning() << "Lexer plugins are NYI";
		return false;
	}
	if(type == "targets") {
		
	}
	return false;
}

void KissScript::log(const QString& from, const QString& message)
{
	std::cout << "Message from plugin " << from.toStdString() << ": " << message.toStdString() << std::endl;
}

void KissScript::log(const QString& message)
{
	std::cout << message.toStdString() << std::endl;
}

QScriptValue KissScript::process()
{
	return engine() ? engine()->newQObject(new Process, QScriptEngine::ScriptOwnership) : QScriptValue();
}