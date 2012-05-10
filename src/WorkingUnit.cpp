#include "WorkingUnit.h"

#include <QDebug>

WorkingUnit::WorkingUnit(const QString& name) : m_name(name), m_parentUnit(0)
{
	
}

Target* WorkingUnit::target()
{
	return m_parentUnit ? m_parentUnit->target() : &m_target;
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

QString WorkingUnit::workingUnitPath() const
{
	return m_name + " -> " + (m_parentUnit ? m_parentUnit->workingUnitPath() : QString("nil"));
}