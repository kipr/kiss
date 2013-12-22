#ifndef _EXTENSION_HELPER_HPP_
#define _EXTENSION_HELPER_HPP_

#include <QString>
#include <QStringList>

namespace kiss
{
  namespace project
  {
    class ExtensionHelper
    {
    public:
      static const QString icon(const QString &file);
      static const QString iconFromExtension(const QString &ext);
      static const bool isFileEditable(const QString &file);
      static const QStringList libExtensions();
    };
  }
}

#endif
