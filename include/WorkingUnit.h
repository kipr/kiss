#ifndef _WORKINGUNIT_H_
#define _WORKINGUNIT_H_

#include <QString>
#include "Target.h"

class WorkingUnit
{
public:
	WorkingUnit(const QString& name);
	
	Target* target();
	
	void setParentUnit(WorkingUnit* parent);
	WorkingUnit* parentUnit() const;
	
	QString workingUnitPath() const;

private:
	QString m_name;
	QString m_port;
	Target m_target;
	WorkingUnit* m_parentUnit;
};

#endif
