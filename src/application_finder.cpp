#include "application_finder.hpp"

#include <QDir>

using namespace kiss;

void ApplicationFinder::addApplicationPath(const QString &name, const QString &path)
{
    _applicationPaths[name].append(path);
}

void ApplicationFinder::clearApplicationPath(const QString &name)
{
    _applicationPaths.remove(name);
}

QList<QString> ApplicationFinder::applicationPath(const QString &name) const
{
    return _applicationPaths[name];
}

ApplicationFinder ApplicationFinder::platform()
{
    ApplicationFinder finder;
    
#ifdef Q_OS_MAC
    {
      finder.addApplicationPath("cs2", "/Applications/cs2.app/Contents/MacOS/cs2");
      QDir c(QDir::current());
      c.cdUp();
      c.cdUp();
      finder.addApplicationPath("cs2", c.absoluteFilePath("cs2.app/Contents/MacOS/cs2"));
      c.cdUp();
      finder.addApplicationPath("cs2", c.absoluteFilePath("cs2.app/Contents/MacOS/cs2"));
    }
#elif defined(Q_OS_WIN)
    {
      QDir c(QDir::current());
      c.cdUp();
      finder.addApplicationPath("cs2", c.absoluteFilePath("cs2/cs2.exe"));
    }
#endif
    return finder;
}