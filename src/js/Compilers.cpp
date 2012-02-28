#include "Compilers.h"

#include "Compiler.h"

Compilers::Compilers()
{
	
}

void Compilers::addCompiler(Compiler* compiler)
{
	if(m_compilers.contains(compiler)) return;
	m_compilers.append(compiler);
}

void Compilers::removeCompiler(Compiler* compiler)
{
	m_compilers.removeAll(compiler);
}

Compiler* Compilers::compiler(const QString& type) const
{
	foreach(Compiler* compiler, m_compilers)
		if(compiler->types().contains(type)) return compiler;
	
	return 0;
}

QList<Compiler*> Compilers::compilers(const QString& type) const
{
	QList<Compiler*> ret;
	foreach(Compiler* compiler, m_compilers)
		if(compiler->types().contains(type)) ret << compiler;
	
	return ret;
}

const QList<Compiler*>& Compilers::compilers() const
{
	return m_compilers;
}