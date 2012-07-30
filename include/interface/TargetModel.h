#ifndef _TARGETMODEL_H_
#define _TARGETMODEL_H_

#include "Target.h"
#include "Interface.h"

#include <QStandardItemModel>
#include <QMap>
#include <QRunnable>
#include <QThreadPool>

class InterfaceManager;

class TargetModel : public QStandardItemModel, public InterfaceResponder
{
Q_OBJECT
public:
	TargetModel(InterfaceManager* manager);
	TargetPtr indexToTarget(const QModelIndex& index) const;
	Interface* indexToInterface(const QModelIndex& index) const;
	
	void refresh();
	
signals:
	void foundTarget(TargetPtr target);
	void interfaceStartedScan(Interface *interface);
	
public slots:
	void addTarget(TargetPtr target);
	
private slots:
	void interfaceRemoved(Interface* interface);
	
private:
	void targetScanStarted(Interface *interface);
	void targetFound(Interface *interface, TargetPtr target);
	
	InterfaceManager* m_manager;
};

#endif
