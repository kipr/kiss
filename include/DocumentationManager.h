#ifndef _DOCUMENTATIONMANAGER_H_
#define _DOCUMENTATIONMANAGER_H_

#include "Singleton.h"

#include <QString>
#include <QList>

class DocumentationLocation
{
public:
	DocumentationLocation(const QString& name, const QString& location,
		const QString& description, const QString& decoration);
	
	const QString& name() const;
	const QString& location() const;
	const QString& description() const;
	const QString& decoration() const;
	
private:
	QString m_name;
	QString m_location;
	QString m_description;
	QString m_decoration;
};

class DocumentationManager : public Singleton<DocumentationManager>
{
public:
	DocumentationManager();
	void addLocation(const QString& name, const QString& location);
	const QList<DocumentationLocation>& locations() const;
	
private:
	static const QString description(const QString& location);
	static const QString decoration(const QString& location);
	QList<DocumentationLocation> m_locations;
};

#endif
