#ifndef _APPLICATION_FINDER_HPP_
#define _APPLICATION_FINDER_HPP_

#include <QString>
#include <QMap>
#include <QList>

namespace kiss
{
    class ApplicationFinder
    {
    public:
        void addApplicationPath(const QString &name, const QString &path);
        void clearApplicationPath(const QString &name);
        QList<QString> applicationPath(const QString &name) const;
        
        static ApplicationFinder platform();
    
    private:
        QMap<QString, QList<QString> > _applicationPaths;
    };
}

#endif
