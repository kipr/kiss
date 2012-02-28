#ifndef _COMPILERS_H_
#define _COMPILERS_H_

#include "Singleton.h"

#include <QList>

class Compiler;

class Compilers : public Singleton<Compilers>
{
public:
	Compilers();
	
	void addCompiler(Compiler* compiler);
	void removeCompiler(Compiler* compiler);
	
	Compiler* compiler(const QString& type) const;
	QList<Compiler*> compilers(const QString& type) const;
	const QList<Compiler*>& compilers() const;
	
private:
	QList<Compiler*> m_compilers;
};

#endif
