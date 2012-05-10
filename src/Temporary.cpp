#include "Temporary.h"
#include "Kiss.h"
#include "FileSystemUtils.h"

QDir Temporary::root()
{
	return QDir(QDir::temp().path() + "/" + TEMPORARY_FOLDER);
}

QDir Temporary::subdir(const QString& name)
{
	QDir p = root();
	if(!p.exists()) QDir().mkpath(p.path());
	return QDir(p.path() + "/" + name);
}

const bool Temporary::clear()
{
	return FileSystemUtils::removeDirectory(root());
}