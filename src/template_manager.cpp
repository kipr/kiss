#include "template_manager.hpp"
#include "template_pack.hpp"

#include <QDir>
#include <QFileInfo>

#include <QDebug>

using namespace Kiss;
using namespace Kiss::Template;

#define TEMPLATE_PACKS "template_packs"

Manager::~Manager()
{
}

void Manager::addPacks(const QString& path)
{
	QFileInfoList entries = QDir(path).entryInfoList(QStringList() << "*.pack", QDir::Files);
	foreach(const QFileInfo& entry, entries) {
		addPack(entry.filePath());
	}
}

bool Manager::addPack(const QString& path)
{
	PackPtr pack = Pack::load(path);
	if(!pack) return false;
	addPack(pack);
	return true;
}

void Manager::addPack(const PackPtr& pack)
{
	m_packs.removeAll(pack);
	m_packs.append(pack);
	
	emit packAdded(pack);
}

bool Manager::removePack(const PackPtr& pack, bool removeAsDefault)
{
	m_packs.removeAll(pack);
	
	emit packRemoved(pack.data());
	
	if(!removeAsDefault) return true;
	
	QDir dir = templatePacksDir();
	if(dir.path() != QFileInfo(pack->loadedFrom()).path()) return true;
	
	return QFile::remove(pack->loadedFrom());
}

bool Manager::removePack(Pack *pack, bool removeAsDefault)
{
	emit packRemoved(pack);
	
	QString loadedFrom = pack->loadedFrom();
	
	QList<PackPtr>::iterator it = m_packs.begin();
	for(; it != m_packs.end(); ++it) {
		if(pack == (*it).data()) {
			it = m_packs.erase(it);
			break;
		}
	}
	
	if(!removeAsDefault) return true;
	
	QDir dir = templatePacksDir();
	if(dir.path() != QFileInfo(loadedFrom).path()) return true;
	
	return QFile::remove(loadedFrom);
}

void Manager::addDefaultPack(const PackPtr& pack)
{
	QString saveName = pack->loadedFrom().isEmpty()
		? pack->name() + ".pack"
		: QFileInfo(pack->loadedFrom()).fileName();
	pack->save(templatePacksDir().filePath(saveName));
}

void Manager::loadDefaultPacks()
{
	addPacks(QDir::currentPath() + "/" + TEMPLATE_PACKS);
}

QList<PackPtr> Manager::packs() const
{
	return m_packs;
}

QDir Manager::templatePacksDir() const
{
	QDir templatePacksDir = QDir::current();
	templatePacksDir.mkpath(TEMPLATE_PACKS);
	templatePacksDir.cd(TEMPLATE_PACKS);
	return templatePacksDir;
}