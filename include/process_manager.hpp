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
        static bool terminate(const QString &name);
        
    private:
        static unsigned long lookupPid(const QString &name);
    };
}

#endif
