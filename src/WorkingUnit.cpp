#include "WorkingUnit.h"

#include <QDebug>

WorkingUnit::WorkingUnit(const QString& name) : m_name(name), m_parentUnit(0)
{
	
}

Target* WorkingUnit::target()
{
	return m_parentUnit ? m_parentUnit->target() : &m_target;
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