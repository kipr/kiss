#ifndef _WORKINGUNIT_H_
#define _WORKINGUNIT_H_

#include <QString>

#include "Device.h"

class WorkingUnit
{
public:
	WorkingUnit(const QString& name);
	
	void setDevice(DevicePtr device);
	DevicePtr device();
	
	const QString& port() const;
	void setPort(const QString& port);
	const bool isPortValid() const;
	
	void setParentUnit(WorkingUnit* parent);
	WorkingUnit* parentUnit() const;
	
	const WorkingUnit* topLevelUnit() const;
	
	QString workingUnitPath() const;

private:
	QString m_name;
	DevicePtr m_device;
	QString m_port;
	WorkingUnit* m_parentUnit;
};

#endif
