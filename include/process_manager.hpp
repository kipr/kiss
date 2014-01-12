#ifndef _PROCESS_MANAGER_HPP_
#define _PROCESS_MANAGER_HPP_

#include <QString>
#include <QProcess>

namespace kiss
{
    class ProcessManager
    {
    public:
        static bool isRunDetectionSupported();
        static bool isRunning(const QString &name);
        static bool start(const QString &path, const QStringList &arguments = QStringList());
        
    private:
        static unsigned long lookupPid(const QString &name);
    };
}

#endif
