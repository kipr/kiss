#ifndef _TARGETDIALOG_H_
#define _TARGETDIALOG_H_

#include "ui_TargetDialog.h"

#include "TargetModel.h"
#include "InterfaceModel.h"

#include <QDialog>

class TargetDialog : public QDialog, private Ui::TargetDialog
{
Q_OBJECT
public:
	TargetDialog(InterfaceManager* manager, QWidget* parent = 0);
	~TargetDialog();
	
	TargetPtr target() const;
	
private slots:
	void currentTargetChanged(const QModelIndex& index);
	void targetChosen(const QModelIndex& index);
	void on_ui_interfaces_currentIndexChanged(int index);
	void on_ui_refresh_clicked();
	
private:
	InterfaceManager *m_manager;
	TargetModel m_model;
	InterfaceModel m_interfaceModel;
};

#endif
