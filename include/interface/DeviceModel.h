#ifndef _DEVICEMODEL_H_
#define _DEVICEMODEL_H_

#include "Device.h"

#include <QStandardItemModel>
#include <QMap>
#include <QRunnable>
#include <QThreadPool>

class Interface;
class InterfaceManager;

class InterfaceWorker : public QObject, public QRunnable
{
Q_OBJECT
public:
	InterfaceWorker(Interface *interface, QStandardItemModel *model);
	~InterfaceWorker();
	void run();
	
signals:
	void foundDevice(DevicePtr device);
	
private:
	Interface *m_interface;
	QStandardItemModel *m_model;
};

class DeviceModel : public QStandardItemModel
{
Q_OBJECT
public:
	DeviceModel(InterfaceManager* manager);
	DevicePtr indexToDevice(const QModelIndex& index) const;
	Interface* indexToInterface(const QModelIndex& index) const;
	
	void filter(Interface* interface);
	
	void refresh();
	
public slots:
	void addDevice(DevicePtr device);
	
private slots:
	void interfaceAdded(Interface* interface);
	void interfaceRemoved(Interface* interface);
	
private:
	void reload(Interface* interface);
	
	InterfaceManager* m_manager;
};

#endif
