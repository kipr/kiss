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
      finder.addApplicationPath("computer", "/Applications/computer.app/Contents/MacOS/computer");
      QDir c(QDir::current());
      c.cdUp();
      c.cdUp();
      finder.addApplicationPath("computer", c.absoluteFilePath("computer.app/Contents/MacOS/computer"));
      c.cdUp();
      finder.addApplicationPath("computer", c.absoluteFilePath("computer.app/Contents/MacOS/computer"));
    }
    {
      finder.addApplicationPath("ks2", "/Applications/ks2.app/Contents/MacOS/ks2");
      QDir c(QDir::current());
      c.cdUp();
      c.cdUp();
      finder.addApplicationPath("ks2", c.absoluteFilePath("ks2.app/Contents/MacOS/ks2"));
      c.cdUp();
      finder.addApplicationPath("ks2", c.absoluteFilePath("ks2.app/Contents/MacOS/ks2"));
    }
#elif defined(Q_OS_WIN)
    {
      QDir c(QDir::current());
      c.cdUp();
      finder.addApplicationPath("computer", c.absoluteFilePath("computer/computer.exe"));
    }
    {
      QDir c(QDir::current());
      c.cdUp();
      finder.addApplicationPath("ks2", c.absoluteFilePath("ks2/ks2.exe"));
    }
#endif
    return finder;
}