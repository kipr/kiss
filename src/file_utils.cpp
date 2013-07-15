#include "file_utils.hpp"

#include <QFile>
#include <QFileInfo>

QString Kiss::FileUtils::absolutePath(const QString &path, const QDir &originDir)
{
	if(QFileInfo(path).isAbsolute()) return path;
	return QDir::cleanPath(originDir.absoluteFilePath(path));
}

QString Kiss::FileUtils::relativePath(const QString &path, const QDir &originDir)
{
	if(QFileInfo(path).isRelative()) return path;
	return originDir.relativeFilePath(path);
}

bool Kiss::FileUtils::remove(const QString &path)
{
	QDir dir(path);

	if(!dir.exists()) return true;

	QFileInfoList entries = dir.entryInfoList(QDir::NoDotAndDotDot
		| QDir::System | QDir::Hidden | QDir::AllDirs | QDir::Files,
		QDir::DirsFirst);

	foreach(const QFileInfo &entry, entries) {
		const QString entryPath = entry.absoluteFilePath();
		if(!(entry.isDir() ? remove(entryPath) : QFile::remove(entryPath))) return false;
	}

	if(!dir.rmdir(path)) return false;

	return true;
}