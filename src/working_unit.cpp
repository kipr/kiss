#include "working_unit.hpp"

#include <QDebug>

using namespace Kiss;

WorkingUnit::WorkingUnit(const QString& name)
	: m_name(name),
	m_parentUnit(0)
{
	
}

void WorkingUnit::setTarget(Target::TargetPtr target)
{
	WorkingUnit *unit = this;
	while(unit->parentUnit()) unit = unit->parentUnit();
	if(unit == this) m_target = target;
	else unit->setTarget(target);
}

Target::TargetPtr WorkingUnit::target()
{
	return m_parentUnit ? m_parentUnit->target() : m_target;
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

WorkingUnit *WorkingUnit::parentUnit() const
{
	return m_parentUnit;
}

const WorkingUnit *WorkingUnit::topLevelUnit() const
{
	const WorkingUnit *unit = this;
	while(unit->parentUnit()) unit = unit->parentUnit();
	return unit;
}

QString WorkingUnit::workingUnitPath() const
{
	return m_name + " -> " + (m_parentUnit ? m_parentUnit->workingUnitPath() : QString("nil"));
}
