#include "FileSystemUtils.h"

bool FileSystemUtils::removeDirectory(const QDir& dir)
{
	return recursiveRemoveDirectory(dir);
}

bool FileSystemUtils::recursiveRemoveDirectory(QDir path)
{
	bool ret = true;
	QFileInfoList files = path.entryInfoList(QDir::Files);
	foreach(const QFileInfo& file, files) ret &= QFile::remove(file.filePath());
	QFileInfoList dirs = path.entryInfoList(QDir::Dirs | QDir::NoDot | QDir::NoDotDot);
	foreach(const QFileInfo& dir, dirs) ret &= recursiveRemoveDirectory(dir.dir());
	QDir().rmdir(path.path());
	return ret;
}