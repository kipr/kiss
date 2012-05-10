#ifndef _WORKINGUNIT_H_
#define _WORKINGUNIT_H_

#include <QString>
#include "Target.h"

class WorkingUnit
{
public:
	WorkingUnit(const QString& name);
	
	Target* target();
	
	const QString& port() const;
	void setPort(const QString& port);
	const bool isPortValid() const;
	
	void setParentUnit(WorkingUnit* parent);
	WorkingUnit* parentUnit() const;
	
	QString workingUnitPath() const;

private:
	QString m_name;
	Target m_target;
	QString m_port;
	WorkingUnit* m_parentUnit;
};

#endif
