#include "documentation_manager.hpp"

#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QDir>
#include <QDebug>

#define DESCRIPTION_FILE "description.txt"
#define DECORATION_FILE "decoration.html"

using namespace Kiss;

DocumentationLocation::DocumentationLocation(const QString& name, const QString& location,
	const QString& description, const QString& decoration)
	: m_name(name),
	m_location(location),
	m_description(description),
	m_decoration(decoration)
{
	
}

const QString& DocumentationLocation::name() const
{
	return m_name;
}

const QString& DocumentationLocation::location() const
{
	return m_location;
}

const QString& DocumentationLocation::description() const
{
	return m_description;
}

const QString& DocumentationLocation::decoration() const
{
	return m_decoration;
}

DocumentationManager::DocumentationManager()
{
	loadDefaultDocumentation();
}

void DocumentationManager::addLocation(const QString& location)
{
	const QString name = QFileInfo(location).fileName();
	QString realLocation = location;
	QFile file(location + "/goto.txt");
	if(file.open(QIODevice::ReadOnly)) {
		realLocation = file.readAll();
		realLocation = realLocation.trimmed();
		realLocation = location + "/" + realLocation;
		file.close();
	}
	qDebug() << "Adding documentation source" << name << "from location" << location;
	m_locations.push_back(DocumentationLocation(name, realLocation,
		description(location), decoration(location)));
}

const QList<DocumentationLocation>& DocumentationManager::locations() const
{
	return m_locations;
}

QString DocumentationManager::documentationPath()
{
	return QDir::currentPath() + "/docs";
}

void DocumentationManager::loadDefaultDocumentation()
{
	qDebug() << "Loading default documentation from" << documentationPath();
	
	QFileInfoList folders = QDir(documentationPath()).entryInfoList(QDir::Dirs | QDir::NoDot | QDir::NoDotDot);
	foreach(const QFileInfo& folder, folders) addLocation(folder.absoluteFilePath());
}

const QString DocumentationManager::description(const QString& location)
{
	QFile file(location + "/" + DESCRIPTION_FILE);
	file.open(QIODevice::ReadOnly);
	QString ret = QTextStream(&file).readAll();
	file.close();
	return ret;
}

const QString DocumentationManager::decoration(const QString& location)
{
	QFile file(location + "/" + DECORATION_FILE);
	file.open(QIODevice::ReadOnly);
	QString ret = QTextStream(&file).readAll();
	file.close();
	return ret;
}