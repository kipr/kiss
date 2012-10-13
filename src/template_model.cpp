#include "template_model.hpp"

#include "template_pack.hpp"
#include "template_manager.hpp"
#include "resource_helper.hpp"

#include <QStandardItem>
#include <QFileInfo>
#include <QDebug>

using namespace Kiss;
using namespace Kiss::Template;

class TemplateItem : public QStandardItem
{
public:
	TemplateItem(const QString& path, Pack *pack, bool readOnly)
		: QStandardItem(QFileInfo(path).fileName()),
		m_path(path),
		m_pack(pack)
	{
		setEditable(!readOnly);
		setIcon(QIcon(":/icon_set/icons/page_white_" + pack->lexer(path) + ".png"));
	}
	
	const QString& path()
	{
		return m_path;
	}
	
	Pack *pack() const
	{
		return m_pack;
	}
	
	template<typename T>
	static TemplateItem *cast(T *obj)
	{
		return dynamic_cast<TemplateItem *>(obj);
	}
	
private:
	const QString m_path;
	Pack *m_pack;
};

class PackItem : public QStandardItem
{
public:
	PackItem(Pack *pack, bool readOnly)
		: QStandardItem(pack->name()),
		m_pack(pack)
	{
		setEditable(!readOnly);
		setIcon(ResourceHelper::ref().icon("folder.png"));
		foreach(const QString& _template, pack->templates()) {
			appendRow(new TemplateItem(_template, pack, readOnly));
		}
	}
	
	Pack *pack() const
	{
		return m_pack;
	}
	
	void removeChild(const QString& path)
	{
		for(int i = 0; i < rowCount(); ++i) {
			TemplateItem *templateItem = TemplateItem::cast(child(i));
			if(!templateItem) continue;
			if(path == templateItem->path()) {
				qDeleteAll(takeRow(i));
				break;
			}
		}
	}
	
	void setChildIcon(const QString& path, const QIcon& icon)
	{
		for(int i = 0; i < rowCount(); ++i) {
			TemplateItem *templateItem = TemplateItem::cast(child(i));
			if(!templateItem) continue;
			if(path == templateItem->path()) {
				templateItem->setIcon(icon);
				break;
			}
		}
	}
	
	template<typename T>
	static PackItem *cast(T *obj)
	{
		return dynamic_cast<PackItem *>(obj);
	}
	
private:
	Pack *m_pack;
};

Model::Model(const Manager *manager, QObject *parent)
	: QStandardItemModel(parent),
	m_manager(manager)
{
	connect(m_manager, SIGNAL(packAdded(Kiss::Template::PackPtr)),
		this, SLOT(packAdded(Kiss::Template::PackPtr)));
	connect(m_manager, SIGNAL(packRemoved(Kiss::Template::Pack *)),
		this, SLOT(packRemoved(Kiss::Template::Pack *)));
	connect(this, SIGNAL(itemChanged(QStandardItem *)), this, SLOT(itemRenamed(QStandardItem *)));
	
	foreach(const PackPtr& pack, manager->packs()) packAdded(pack);
}
	
Pack *Model::indexToPack(const QModelIndex& index) const
{
	QStandardItem *item = itemFromIndex(index);
	
	TemplateItem *templateItem = TemplateItem::cast(item);
	if(templateItem) return templateItem->pack();
	
	PackItem *packItem = PackItem::cast(item);
	if(packItem) return packItem->pack();
	
	return 0;
}

Template::File Model::indexToFile(const QModelIndex& index) const
{
	TemplateItem *templateItem = TemplateItem::cast(itemFromIndex(index));
	if(!templateItem) return Template::File();
	
	return templateItem->pack()->file(templateItem->path());
}

bool Model::isIndexPack(const QModelIndex& index) const
{
	return PackItem::cast(itemFromIndex(index));
}

bool Model::isIndexFile(const QModelIndex& index) const
{
	return TemplateItem::cast(itemFromIndex(index));
}

void Model::setReadOnly(const bool& readOnly)
{
	if(readOnly == m_readOnly) return;
	m_readOnly = readOnly;
	setReadOnly(invisibleRootItem());
}

const bool& Model::isReadOnly() const
{
	return m_readOnly;
}

void Model::packAdded(const Kiss::Template::PackPtr& pack)
{
	appendRow(new PackItem(pack.data(), m_readOnly));
	
	connect(pack.data(), SIGNAL(nameChanged(QString)),
		this, SLOT(packNameChanged(QString)));
	connect(pack.data(), SIGNAL(fileAdded(QString, Kiss::Template::File)),
		this, SLOT(packFileAdded(QString, Kiss::Template::File)));
	connect(pack.data(), SIGNAL(fileRemoved(QString)),
		this, SLOT(packFileRemoved(QString)));
}

void Model::packRemoved(Kiss::Template::Pack *pack)
{
	if(!pack) return;
	
	pack->disconnect(this);
	
	qDebug() << "Pack remove";
	QStandardItem *root = invisibleRootItem();
	for(int i = 0; i < root->rowCount(); ++i) {
		PackItem *packItem = PackItem::cast(root->child(i));
		if(!packItem) continue;
		if(pack == packItem->pack()) {
			qDebug() << "Removing" << i;
			root->removeRow(i);
		}
	}
}

void Model::packNameChanged(const QString& name)
{
	Pack *pack = qobject_cast<Pack *>(sender());
	
	QStandardItem *root = invisibleRootItem();
	for(int i = 0; i < root->rowCount(); ++i) {
		PackItem *packItem = PackItem::cast(root->child(i));
		if(!packItem) continue;
		if(pack == packItem->pack()) {
			packItem->setText(name);
		}
	}
}

void Model::packFileAdded(const QString& path, const Kiss::Template::File& file)
{
	Pack *pack = qobject_cast<Pack *>(sender());
	if(!pack) return;
	
	QStandardItem *root = invisibleRootItem();
	for(int i = 0; i < root->rowCount(); ++i) {
		PackItem *packItem = PackItem::cast(root->child(i));
		if(!packItem) continue;
		if(pack == packItem->pack()) {
			packItem->appendRow(new TemplateItem(path, pack, m_readOnly));
		}
	}
}

void Model::packFileRemoved(const QString& path)
{
	Pack *pack = qobject_cast<Pack *>(sender());
	if(!pack) return;
	
	QStandardItem *root = invisibleRootItem();
	for(int i = 0; i < root->rowCount(); ++i) {
		PackItem *packItem = PackItem::cast(root->child(i));
		if(!packItem) continue;
		if(pack == packItem->pack()) {
			packItem->removeChild(path);
		}
	}
}

void Model::itemRenamed(QStandardItem *item)
{
	PackItem *packItem = PackItem::cast(item);
	
	if(packItem) {
		packItem->pack()->setName(packItem->text());
		return;
	}
	TemplateItem *templateItem = TemplateItem::cast(item);
	
	if(templateItem) {
		QFileInfo info(templateItem->path());
		templateItem->pack()->rename(templateItem->path(),
			(info.path() != "." ? info.path() + "/" : "") + templateItem->text());
		return;
	}
}

void Model::setReadOnly(QStandardItem *item)
{
	item->setEditable(!m_readOnly);
	for(int i = 0; i < item->rowCount(); ++i) {
		setReadOnly(item->child(i));
	}
}