#include "DocumentationManager.h"

#include <QFile>
#include <QTextStream>

#define DESCRIPTION_FILE "description.txt"
#define DECORATION_FILE "decoration.html"

DocumentationLocation::DocumentationLocation(const QString& name, const QString& location,
	const QString& description, const QString& decoration)
	: m_name(name), m_location(location), m_description(description), m_decoration(decoration)
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
	
}

void DocumentationManager::addLocation(const QString& name, const QString& location)
{
	m_locations.push_back(DocumentationLocation(name, location,
		description(location), decoration(location)));
}

const QList<DocumentationLocation>& DocumentationManager::locations() const
{
	return m_locations;
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