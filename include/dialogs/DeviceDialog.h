#ifndef _DEVICEDIALOG_H_
#define _DEVICEDIALOG_H_

#include "ui_DeviceDialog.h"

#include "DeviceModel.h"
#include "InterfaceModel.h"

#include <QDialog>

class DeviceDialog : public QDialog, private Ui::DeviceDialog
{
Q_OBJECT
public:
	DeviceDialog(InterfaceManager* manager, QWidget* parent = 0);
	~DeviceDialog();
	
	DevicePtr device() const;
	
private slots:
	void currentDeviceChanged(const QModelIndex& index);
	void deviceChosen(const QModelIndex& index);
	void on_ui_interfaces_currentIndexChanged(int index);
	void on_ui_refresh_clicked();
	
private:
	InterfaceManager *m_manager;
	DeviceModel m_model;
	InterfaceModel m_interfaceModel;
};

#endif
