#include "template_manager.hpp"
#include "template_pack.hpp"

#include <QDir>
#include <QFileInfo>

using namespace Kiss;
using namespace Kiss::Template;

#define TEMPLATE_PACKS "template_packs"

Manager::~Manager()
{
	qDeleteAll(m_packs);
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
	Pack *pack = Pack::load(path);
	if(!pack) return false;
	addPack(pack);
	return true;
}

void Manager::addPack(Pack *pack)
{
	m_packs.removeAll(pack);
	m_packs.append(pack);
}

void Manager::removePack(Pack *pack)
{
	m_packs.removeAll(pack);
}

void Manager::addDefaultPack(Pack *pack)
{
	pack->save(QDir::currentPath() + "/" + TEMPLATE_PACKS + "/"
		+ (pack->loadedFrom().isEmpty() ? pack->name() + ".pack" : pack->loadedFrom()));
}

void Manager::loadDefaultPacks()
{
	addPacks(QDir::currentPath() + "/" + TEMPLATE_PACKS);
}

QList<Pack *> Manager::packs() const
{
	return m_packs;
}