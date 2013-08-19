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
    finder.addApplicationPath("computer", "/Applications/computer.app/Contents/MacOS/computer");
#elif defined(Q_OS_WIN)
    QDir c(QDir::current());
    c.cdUp();
    finder.addApplicationPath("computer", c.absoluteFilePath("computer"));
#endif
    return finder;
}