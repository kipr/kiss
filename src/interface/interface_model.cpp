#include "interface_model.hpp"

#include "interface.hpp"
#include "interface_manager.hpp"

using namespace Kiss::Target;

class InterfaceItem : public QStandardItem
{
public:
	InterfaceItem(Interface *interface, const bool& all = false)
		: QStandardItem(all ? "All Interfaces" : interface->name()),
		m_interface(interface),
		m_all(all)
	{}
	
	Interface *interface()
	{
		return m_interface;
	}
	
	const bool& all()
	{
		return m_all;
	}
	
	template<typename T>
	static Interface* interface_cast(T *t)
	{
		InterfaceItem* item = dynamic_cast<InterfaceItem*>(t);
		return item ? item->interface() : 0;
	}
private:
	Interface *m_interface;
	bool m_all;
};

InterfaceModel::InterfaceModel(InterfaceManager *manager, QObject *parent)
	: QStandardItemModel(parent),
	m_manager(manager)
{
	connect(m_manager, SIGNAL(interfaceAdded(Interface *)), SLOT(interfaceAdded(Interface *)));
	connect(m_manager, SIGNAL(interfaceRemoved(Interface *)), SLOT(interfaceRemoved(Interface *)));
	reload();
}

Interface* InterfaceModel::rowToInterface(const int& row) const
{
	return InterfaceItem::interface_cast(item(row));
}

Interface* InterfaceModel::indexToInterface(const QModelIndex& index) const
{
	return InterfaceItem::interface_cast(itemFromIndex(index));
}

void InterfaceModel::setAllInterface(const bool& on)
{
	if(on) {
		setAllInterface(false);
		insertRow(0, new InterfaceItem(0, true));
	} else {
		InterfaceItem *i = dynamic_cast<InterfaceItem *>(item(0));
		if(i && i->all()) delete takeItem(0);
	}
}

void InterfaceModel::interfaceAdded(Interface *interface)
{
	appendRow(new InterfaceItem(interface));
}

void InterfaceModel::interfaceRemoved(Interface *interface)
{
	for(int i = 0; i < rowCount(); ++i) {
		if(InterfaceItem::interface_cast(item(i)) == interface) delete takeItem(i--);
	}
}

void InterfaceModel::reload()
{
	clear();
	foreach(Interface* interface, m_manager->interfaces()) interfaceAdded(interface);
}