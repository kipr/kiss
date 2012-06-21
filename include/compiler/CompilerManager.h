#ifndef _COMPILERMANAGER_H_
#define _COMPILERMANAGER_H_

#include "Singleton.h"

#include <QList>
#include <QStringList>

class Compiler;

class CompilerManager : public Singleton<CompilerManager>
{
public:
	void addCompiler(Compiler* compiler);
	void removeCompiler(Compiler* compiler);
	const QList<Compiler*>& compilers() const;
	
	const QStringList extensions() const;
	const QStringList formattedExtensions() const;
private:
	QList<Compiler*> m_compilers;
};

#endif
