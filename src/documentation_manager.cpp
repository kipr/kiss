#include "documentation_manager.hpp"

#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QSettings>
#include <QDebug>

#define DESCRIPTION_FILE "description.txt"
#define DECORATION_FILE "decoration.html"
#define METADATA_FILE "metadata.ini"

using namespace kiss;

DocumentationSource::DocumentationSource(const QString &path)
    : _path(path)
{
}

QMap<QString, QString> DocumentationSource::metadata() const
{
    if(_metadata.isEmpty()) lazyInitMetadata();
    return _metadata;
}

bool DocumentationSource::operator ==(const DocumentationSource &rhs) const
{
    return _path == rhs._path;
}

void DocumentationSource::lazyInitMetadata() const
{
    _metadata.clear();
    QSettings settings(QDir(_path).absoluteFilePath(METADATA_FILE), QSettings::IniFormat);
    Q_FOREACH(const QString &key, settings.allKeys()) {
        _metadata[key] = settings.value(key).toString().replace("${PATH}", _path);
    }
}

const QString &DocumentationSource::path() const
{
    return _path;
}

DocumentationManager::DocumentationManager()
{
	loadDefaultDocumentationSources();
}

void DocumentationManager::addDocumentationSource(const DocumentationSource &documentationSource)
{
    _documentationSources.append(documentationSource);
}

void DocumentationManager::removeDocumentationSource(const DocumentationSource &documentationSource)
{
    _documentationSources.removeAll(documentationSource);
}

const QList<DocumentationSource> &DocumentationManager::documentationSources() const
{
    return _documentationSources;
}

QString DocumentationManager::documentationPath()
{
	return QDir::current().absoluteFilePath("docs");
}

void DocumentationManager::loadDefaultDocumentationSources()
{
	QFileInfoList folders = QDir(documentationPath()).entryInfoList(QDir::Dirs | QDir::NoDot | QDir::NoDotDot);
	foreach(const QFileInfo &folder, folders) addDocumentationSource(folder.absoluteFilePath());
}