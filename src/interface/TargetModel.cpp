#include "TargetModel.h"

#include "Target.h"
#include "Interface.h"
#include "InterfaceManager.h"

#include "ResourceHelper.h"

#include <QDebug>

class TargetItem : public QStandardItem
{
public:
	TargetItem(TargetPtr target)
		: QStandardItem(target->displayName()), m_target(target)
	{
		setEditable(false);
		setIcon(ResourceHelper::ref().icon("default_target"));
	}
	
	TargetPtr target() {
		return m_target;
	}
	
	template<typename T>
	static TargetPtr target_cast(T* t) {
		TargetItem* item = dynamic_cast<TargetItem*>(t);
		return item ? item->target() : TargetPtr();
	}
	
	template<typename T>
	static Interface* interface_cast(T* t) {
		TargetPtr target = target_cast(t);
		return target.get() ? target->interface() : 0;
	}
private:
	TargetPtr m_target;
};

TargetModel::TargetModel(InterfaceManager* manager) : m_manager(manager)
{
	connect(m_manager, SIGNAL(interfaceRemoved(Interface *)), SLOT(interfaceRemoved(Interface *)), Qt::QueuedConnection);
	connect(this, SIGNAL(interfaceStartedScan(Interface *)), SLOT(interfaceRemoved(Interface *)), Qt::QueuedConnection);
	connect(this, SIGNAL(foundTarget(TargetPtr)), SLOT(addTarget(TargetPtr)), Qt::QueuedConnection);
}

TargetPtr TargetModel::indexToTarget(const QModelIndex& index) const
{
	return TargetItem::target_cast(itemFromIndex(index));
}

Interface* TargetModel::indexToInterface(const QModelIndex& index) const
{
	return TargetItem::interface_cast(itemFromIndex(index));
}

void TargetModel::refresh()
{
	clear();
	// foreach(Interface *interface, InterfaceManager::ref().interfaces()) interface->scan(this);
}

void TargetModel::addTarget(TargetPtr target)
{
	appendRow(new TargetItem(target));
}

void TargetModel::interfaceRemoved(Interface* interface)
{
	for(int i = 0; i < rowCount(); ++i) {
		if(TargetItem::interface_cast(item(i)) == interface) delete takeItem(i--);
	}
}

void TargetModel::targetScanStarted(Interface *interface)
{
	emit interfaceStartedScan(interface);
}

void TargetModel::targetFound(Interface *interface, TargetPtr target)
{
	emit foundTarget(target);
}
