#ifndef _FILE_UTILS_HPP_
#define _FILE_UTILS_HPP_

#include <QString>

namespace Kiss
{
	class FileUtils
	{
	public:
		static bool remove(const QString& path);
	};
}

#endif
