#include "process_manager.hpp"

#include <QtGlobal>
#include <QDebug>
#include <QFileInfo>
#include <QProcess>

#ifdef Q_OS_MAC
#import <sys/proc_info.h>
#import <libproc.h>
#elif defined(Q_OS_WIN)

#else
#warning Compiling without some process management features
#endif

using namespace kiss;

bool ProcessManager::isRunDetectionSupported()
{
#ifdef Q_OS_MAC
    return true;
#elif defined(Q_OS_WIN)
    return false;
#endif
    return false;
}

bool ProcessManager::isRunning(const QString &name)
{
    return lookupPid(name) > 0;
}

bool ProcessManager::start(const QString &path, const QStringList &arguments)
{
    return QProcess::startDetached(path, arguments);
}

bool ProcessManager::terminate(const QString &name)
{
#ifdef Q_OS_MAC
    Q_PID pid = lookupPid(name);
    if(!pid) return false;
    int sig = 0;
    proc_terminate(pid, &sig);
#elif defined(Q_OS_WIN)
	return false;
#else
    return false;
#endif
}

Q_PID ProcessManager::lookupPid(const QString &name)
{
#ifdef Q_OS_MAC
    const int procnum = proc_listpids(PROC_ALL_PIDS, 0, NULL, 0);
    pid_t pids[1024];
    bzero(pids, 1024);
    proc_listpids(PROC_ALL_PIDS, 0, pids, sizeof pids);
    for(int i = 0; i < procnum; ++i) {
        if(!pids[i]) continue;
        char pathBuffer[PROC_PIDPATHINFO_MAXSIZE];
        bzero(pathBuffer, PROC_PIDPATHINFO_MAXSIZE);
        proc_pidpath(pids[i], pathBuffer, sizeof pathBuffer);
        QString path(pathBuffer);
        if(path.isEmpty()) continue;
        if(name == QFileInfo(path).fileName()) return pids[i];
    }
#elif defined(Q_OS_WIN)

#endif
    return 0;
}