#include "Compilation.h"

#include "Compiler.h"
#include "ProjectManager.h"
#include "PlatformHintsManager.h"

#include <QDebug>
#include <QFileInfo>
#include <QStringList>
#include <QFileInfo>

Compilation::Compilation(const QList<Compiler*>& compilers, const QMap<QString, QString>& settings, const QString& platform)
	: m_compilers(compilers), m_settings(settings), m_name(""), m_results(true)
{
	m_settings = unite(m_settings, PlatformHintsManager::ref().flagMap(platform));
}

Compilation::Compilation(const QList<Compiler*>& compilers, Project* project, const QString& platform)
	: m_compilers(compilers), m_settings(project->settings()), m_results(true)
{
	m_name = project->name();
	addFiles(ProjectManager::ref().archiveWriter(project)->files());
	m_settings = unite(m_settings, PlatformHintsManager::ref().flagMap(platform));
}

Compilation::Compilation(const QList<Compiler*>& compilers, const QString& file, const QString& platform)
	: m_compilers(compilers), m_settings(PlatformHintsManager::ref().flagMap(platform)), m_results(true)
{
	m_name = QFileInfo(file).baseName();
	addFile(file);
}

void Compilation::addFile(const QString& file, bool remove)
{
	if(QFileInfo(file).completeSuffix().isEmpty()) {
		qWarning() << "Discarding file" << file << "from compilation because it has no extension.";
		return;
	}
	m_files += file;
	if(remove) m_removes.push_back(file);
}

void Compilation::addFiles(const QStringList& files, bool remove)
{
	foreach(const QString& file, files) addFile(file, remove);
}

const CompileResult& Compilation::results()
{
	return m_results;
}

void Compilation::addCompileResult(const QString& file)
{
	m_compileResults.push_back(file);
}

const QStringList& Compilation::compileResults() const
{
	return m_compileResults;
}

void Compilation::setName(const QString& name)
{
	m_name = name;
}

const QString& Compilation::name() const
{
	return m_name;
}

const QMap<QString, QString>& Compilation::settings() const
{
	return m_settings;
}

const bool Compilation::start()
{
	qDebug() << "Compilation starting with" << m_files;
	while(!m_files.isEmpty()) {
		const QStringList files = m_files.values();
		const QString ext = QFileInfo(files[0]).completeSuffix();
		const QStringList filtered = files.filter(QRegExp(QString(".+\\.") + ext));
		if(!compile(filtered, compilerFor(ext))) return false;
		printf("Looping...\n");
	}
	// TODO: foreach(const QString& remove, m_removes) QFile::remove(remove);
	return true;
}

const bool Compilation::compile(const QStringList& files, Compiler* compiler)
{
	m_files -= QSet<QString>::fromList(files);
	if(!compiler) {
		qWarning() << "Compilation does not know how to compile" << files;
		return true;
	}
	qDebug() << "Compiling" << files << "with" << compiler->name();
	CompileResult result = compiler->compile(this, files);
	qDebug() << result.categorizedOutput();
	m_results += result;
	return result.success();
}

Compiler* Compilation::compilerFor(const QString& ext)
{
	foreach(Compiler* compiler, m_compilers) {
		if(compiler->types().contains(ext)) return compiler;
	}
	return 0;
}

QMap<QString, QString> Compilation::unite(const QMap<QString, QString>& a, const QMap<QString, QString>& b) const
{
	QMap<QString, QString> ret = a;
	ret = ret.unite(b);
	foreach(const QString& key, ret.keys()) {
		QStringList values = QStringList(ret.values(key));
		if(values.size() == 1) continue;
		ret.remove(key);
		ret[key] = values.join(" ");
	}
	return ret;
}
