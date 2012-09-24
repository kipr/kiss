#ifndef _WORKING_UNIT_HPP_
#define _WORKING_UNIT_HPP_

#include <QString>

#include "target.hpp"

namespace Kiss
{
	class WorkingUnit
	{
	public:
		WorkingUnit(const QString& name);

		void setTarget(Target::TargetPtr target);
		Target::TargetPtr target();

		const QString& port() const;
		void setPort(const QString& port);
		const bool isPortValid() const;

		void setParentUnit(WorkingUnit *parent);
		WorkingUnit *parentUnit() const;

		const WorkingUnit *topLevelUnit() const;

		QString workingUnitPath() const;

	private:
		QString m_name;
		Target::TargetPtr m_target;
		QString m_port;
		WorkingUnit* m_parentUnit;
	};
}

#endif
