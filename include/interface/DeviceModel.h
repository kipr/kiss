#ifndef _DEVICEMODEL_H_
#define _DEVICEMODEL_H_

#include "Device.h"
#include "Interface.h"

#include <QStandardItemModel>
#include <QMap>
#include <QRunnable>
#include <QThreadPool>

class InterfaceManager;

class DeviceModel : public QStandardItemModel, public InterfaceResponder
{
Q_OBJECT
public:
	DeviceModel(InterfaceManager* manager);
	DevicePtr indexToDevice(const QModelIndex& index) const;
	Interface* indexToInterface(const QModelIndex& index) const;
	
	void refresh();
	
signals:
	void foundDevice(DevicePtr device);
	void interfaceStartedScan(Interface *interface);
	
public slots:
	void addDevice(DevicePtr device);
	
private slots:
	void interfaceRemoved(Interface* interface);
	
private:
	void deviceScanStarted(Interface *interface);
	void deviceFound(Interface *interface, DevicePtr device);
	
	InterfaceManager* m_manager;
};

#endif
