#ifndef _EXTENSION_HELPER_HPP_
#define _EXTENSION_HELPER_HPP_

#include <QString>
#include <QStringList>

namespace Kiss
{
	namespace Project
	{
		class ExtensionHelper
		{
		public:
			static QString icon(const QString &file);
			static bool isFileEditable(const QString &file);
			static const QStringList libExtensions();
		};
	}
}

#endif