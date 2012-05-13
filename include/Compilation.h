#ifndef _COMPILATION_H_
#define _COMPILATION_H_

#include "Project.h"
#include "Compiler.h"

#include <QString>
#include <QStringList>
#include <QMap>
#include <QSet>

class Compilation
{
public:
	Compilation(const QList<Compiler*>& compilers, const QMap<QString, QString>& settings);
	Compilation(const QList<Compiler*>& compilers, Project* project);
	Compilation(const QList<Compiler*>& compilers, const QString& file);
	
	void addFile(const QString& file, bool remove = false);
	void addFiles(const QStringList& files, bool remove = false);
	
	const CompileResult& results();
	
	void addCompileResult(const QString& file);
	const QStringList& compileResults() const;
	
	void setName(const QString& name);
	const QString& name() const;
	const QMap<QString, QString>& settings() const;
	
	const bool start();
protected:
	const bool compile(const QStringList& files, Compiler* compiler);
	Compiler* compilerFor(const QString& ext);

private:
	QList<Compiler*> m_compilers;
	QMap<QString, QString> m_settings;
	QString m_name;
	QSet<QString> m_files;
	QStringList m_compileResults;
	QStringList m_removes;
	CompileResult m_results;
};

#endif
