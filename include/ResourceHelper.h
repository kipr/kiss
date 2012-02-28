#ifndef _RESOURCE_HELPER_H_
#define _RESOURCE_HELPER_H_

#include "Singleton.h"

#include <QStringList>
#include <QMap>
#include <QIcon>
#include <QPixmap>

struct ResourceHelper : Singleton<ResourceHelper>
{
	ResourceHelper();
	
	QString lookup(const QString& name);
	QIcon icon(const QString& name);
	QPixmap pixmap(const QString& name);
	
	const QStringList& searchLocations();
	void addSearchLocation(const QString& location);
	
private:
	QMap<QString, QString> m_cache;
	
	QStringList m_locations;
};

#endif
