#include "WorkingUnit.h"

#include <QDebug>

WorkingUnit::WorkingUnit(const QString& name) : m_name(name), m_parentUnit(0)
{
	
}

void WorkingUnit::setDevice(DevicePtr device)
{
	WorkingUnit* unit = this;
	while(unit->parentUnit()) unit = unit->parentUnit();
	if(unit == this) m_device = device;
	else 
	
	unit->setDevice(device);
}

DevicePtr WorkingUnit::device()
{
	return m_parentUnit ? m_parentUnit->device() : m_device;
}

const QString& WorkingUnit::port() const
{
	return m_port;
}

void WorkingUnit::setPort(const QString& port)
{
	m_port = port;
}

const bool WorkingUnit::isPortValid() const
{
	return !m_port.isEmpty();
}

void WorkingUnit::setParentUnit(WorkingUnit* parent)
{
	m_parentUnit = parent;
}

WorkingUnit* WorkingUnit::parentUnit() const
{
	return m_parentUnit;
}

const WorkingUnit* WorkingUnit::topLevelUnit() const
{
	const WorkingUnit* unit = this;
	while(unit->parentUnit()) unit = unit->parentUnit();
	return unit;
}

QString WorkingUnit::workingUnitPath() const
{
	return m_name + " -> " + (m_parentUnit ? m_parentUnit->workingUnitPath() : QString("nil"));
}