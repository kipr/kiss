#include "CompilerManager.h"

#include "Compiler.h"

void CompilerManager::addCompiler(Compiler* compiler)
{
	m_compilers.push_back(compiler);
}

void CompilerManager::removeCompiler(Compiler* compiler)
{
	m_compilers.removeAll(compiler);
}

const QList<Compiler*>& CompilerManager::compilers() const
{
	return m_compilers;
}

const QStringList CompilerManager::extensions() const
{
	QStringList ret;
	foreach(Compiler* compiler, m_compilers) {
		ret << compiler->types();
	}
	ret.removeDuplicates();
	return ret;
}

// This is pretty jank. Could probably be done better using some type of metadata.
const QStringList CompilerManager::formattedExtensions() const
{
	QStringList ret;
	foreach(const QString& type, extensions()) {
		ret << type.toUpper() + " Files (*." + type + ")";
	}
	return ret;
}