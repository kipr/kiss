#ifndef _TEMPORARY_H_
#define _TEMPORARY_H_

#include "Singleton.h"
#include <QDir>

class Temporary
{
public:
	static QDir root();
	static QDir subdir(const QString& name);
	static const bool clear();
};

#endif
