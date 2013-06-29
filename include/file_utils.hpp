#ifndef _FILE_UTILS_HPP_
#define _FILE_UTILS_HPP_

#include <QDir>
#include <QString>

namespace Kiss
{
	class FileUtils
	{
	public:
		static QString absolutePath(const QString &path, const QDir &originDir);
		static QString relativePath(const QString &path, const QDir &originDir);
		static bool remove(const QString& path);
	};
}

#endif
