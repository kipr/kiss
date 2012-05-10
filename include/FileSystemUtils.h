#ifndef _FILESYSTEMUTILS_H_
#define _FILESYSTEMUTILS_H_

#include <QDir>

class FileSystemUtils
{
public:
	static bool removeDirectory(const QDir& dir);
private:
	static bool recursiveRemoveDirectory(QDir path);
};

#endif
