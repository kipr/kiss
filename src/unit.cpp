#include "unit.hpp"

#include "communication_manager.hpp"

#include <kar.hpp>

#include <QFileInfo>
#include <QDebug>

using namespace Kiss;

Unit::Unit(Unit *parent)
	: m_parent(0)
{
	setParent(parent);
}

void Unit::setParent(Unit *parent)
{
	if(m_parent == parent) return;
	if(m_parent) m_parent->removeChild(this);
	m_parent = parent;
	if(m_parent) m_parent->addChild(this);
}

Unit *Unit::parent() const
{
	return m_parent;
}

void Unit::addChild(Unit *child)
{
	if(m_children.contains(child)) return;
	child->setParent(this);
	m_children.append(child);
}

void Unit::removeChild(Unit *child)
{
	m_children.removeAll(child);
}

const QList<Unit *>& Unit::children() const
{
	return m_children;
}

Unit *Unit::top()
{
	return m_parent ? m_parent->top() : this;
}

const Unit *Unit::top() const
{
	return m_parent ? m_parent->top() : this;
}

bool Unit::execute(const Unit::Action &action) const
{
	if(top() != this) return top()->execute(action);
	
	using namespace Target;
	
	QString name = QFileInfo(m_name).baseName();
	
	if(action == Unit::Download) {
		qDebug() << "Unit" << m_name << "download";
		Kiss::KarPtr package = archive();
		if(package.isNull()) return false;
		CommunicationManager::ref().admit(CommunicationEntryPtr(
			new CommunicationEntry(target(), CommunicationEntry::Download,
			name, package)));
		return true;
	}
	
	if(action == Unit::Compile) {
		qDebug() << "Unit" << m_name << "compile";
		CommunicationManager::ref().admit(CommunicationEntryPtr(
			new CommunicationEntry(target(), CommunicationEntry::Compile, name)));
		return true;
	}
	
	if(action == Unit::Run) {
		qDebug() << "Unit" << m_name << "run";
		CommunicationManager::ref().admit(CommunicationEntryPtr(
			new CommunicationEntry(target(), CommunicationEntry::Run, name)));
		return true;
	}
	
	qWarning() << "Unit doesn't know how to execute action" << action;
	return false;
}

void Unit::setName(const QString& name)
{
	m_name = name;
}

const QString& Unit::name() const
{
	return m_name;
}

QString Unit::fullName() const
{
	if(!m_parent) return m_name;
	return m_name + " (" + m_parent->name() + ")";
}

void Unit::setTarget(const Target::TargetPtr& target)
{
	if(top() == this) m_target = target;
	else top()->setTarget(target);
}

Target::TargetPtr Unit::target() const
{
	if(top() == this) return m_target;
	return top()->target();
}

bool Unit::unitActionRequested()
{
	foreach(Unit *child, m_children) {
		if(!child->unitActionRequested()) return false;
	}
	return true;
}
