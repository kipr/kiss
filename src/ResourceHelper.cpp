#include "ResourceHelper.h"

#include <QDir>
#include <QDebug>

ResourceHelper::ResourceHelper()
{
	addSearchLocation(":/shortcuts/target/icon_set/icons/");
	addSearchLocation(":/shortcuts/edit/icon_set/icons/");
	addSearchLocation(":/debugger/icon_set/icons/");
	addSearchLocation(":/shortcuts/file/icon_set/icons/");
	addSearchLocation(":/browser/icon_set/icons/");
	addSearchLocation(":/icon_set/icons/");
}

QString ResourceHelper::lookup(const QString& name)
{
	if(name.isEmpty()) return "";
	
	QMap<QString, QString>::iterator it = m_cache.find(name);
	if(it != m_cache.end()) return *it;
	
	QDir dir;
	foreach(const QString& location, m_locations) {
		dir.setPath(location);
		QFileInfoList entries = dir.entryInfoList(QStringList() << QString("*") + name + "*");
		if(entries.size() == 0) continue;
		const QString& path = entries[0].canonicalFilePath();
		if(entries.size() > 1) {
			qWarning() << "More than one resource in" << dir.path() << "matches pattern" << name;
			qWarning() << "\tUsing" << path;
		}
		m_cache[name] = path;
		return path;
	}
	
	qWarning() << "ResourceHelper Failed to lookup resource" << name << "(tried" << m_locations.size() << "locations)";
	
	return "";
}

QIcon ResourceHelper::icon(const QString& name)
{
	return QIcon(lookup(name));
}

QPixmap ResourceHelper::pixmap(const QString& name)
{
	return QPixmap(lookup(name));
}

const QStringList& ResourceHelper::searchLocations()
{
	return m_locations;
}

void ResourceHelper::addSearchLocation(const QString& location)
{
	m_locations.append(location);
}