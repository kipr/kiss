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
    QDir c(QDir::current());
#ifdef Q_OS_MAC
    finder.addApplicationPath("computer", "/Applications/computer.app/Contents/MacOS/computer");
    c.cdUp();
    c.cdUp();
    finder.addApplicationPath("computer", c.absoluteFilePath("computer.app/Contents/MacOS/computer"));
    c.cdUp();
    finder.addApplicationPath("computer", c.absoluteFilePath("computer.app/Contents/MacOS/computer"));
#elif defined(Q_OS_WIN)
    c.cdUp();
    finder.addApplicationPath("computer", c.absoluteFilePath("computer/computer.exe"));
#endif
    return finder;
}