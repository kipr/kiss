#ifndef _NAMED_H_
#define _NAMED_H_

#include <QString>

class Named
{
public:
	Named(const QString& name);
	const QString& name() const;
private:
	const QString m_name;
};

#endif
