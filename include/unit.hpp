#ifndef _UNIT_HPP_
#define _UNIT_HPP_

#include <QList>
#include <QString>

#include "target.hpp"

#include <kar.hpp>

namespace Kiss
{
	class Unit
	{
	public:
		enum Action {
			Download,
			Compile,
			Run
		};
		
		Unit(Unit *parent = 0);
		
		void setParent(Unit *parent);
		Unit *parent() const;
		
		void addChild(Unit *child);
		void removeChild(Unit *child);
		
		const QList<Unit *>& children() const;
		
		Unit *top();
		const Unit *top() const;
		
		virtual bool execute(const Action &action) const;
		
		void setName(const QString& name);
		const QString& name() const;
		QString fullName() const;
		
		void setTarget(const Target::TargetPtr& target);
		Target::TargetPtr target() const;
		
		virtual bool unitActionRequested();
		
		virtual Kiss::KarPtr archive() const = 0;
		
	private:
		Unit *m_parent;
		QList<Unit *> m_children;
		Target::TargetPtr m_target;
		QString m_name;
	};
}

#endif
