#ifndef _SYSTEM_UTILS_HPP_
#define _SYSTEM_UTILS_HPP_

#include <QString>

namespace kiss
{
  class SystemUtils
  {
  public:
    static bool supportsMoveToTrash();
    static bool moveToTrash(const QString &path);
  };
}

#endif
