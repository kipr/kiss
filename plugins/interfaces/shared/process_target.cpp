#include "process_target.hpp"
#include "process_manager.hpp"

#include <QFileInfo>

using namespace kiss;
using namespace kiss::target;

ProcessTarget::ProcessTarget(const TargetPtr &target, const QList<QString> &executablePaths)
    : Target(target->interface(), "Process Target")
    , _target(target)
    , _executablePaths(executablePaths)
{
}

QMap<QString, QString> ProcessTarget::information() const
{
    ensureStarted();
    return _target->information();
}

bool ProcessTarget::disconnect()
{
    ensureStarted();
    return _target->disconnect();
}

bool ProcessTarget::available()
{
    ensureStarted();
    return _target->available();
}

Target::ReturnCode ProcessTarget::compile(quint64 id, const QString& name)
{
    ensureStarted();
    return _target->compile(id, name);
}

Target::ReturnCode ProcessTarget::download(quint64 id, const QString& name, const kiss::KarPtr& archive)
{
    ensureStarted();
    return _target->download(id, name, archive);
}

Target::ReturnCode ProcessTarget::run(quint64 id, const QString& name)
{
    ensureStarted();
    return _target->run(id, name);
}

Target::ReturnCode ProcessTarget::list(quint64 id)
{
    ensureStarted();
    return _target->list(id);
}

Target::ReturnCode ProcessTarget::deleteProgram(quint64 id, const QString& name)
{
    ensureStarted();
    return _target->deleteProgram(id, name);
}

Target::ReturnCode ProcessTarget::interaction(quint64 id, const QString& command)
{
    ensureStarted();
    return _target->interaction(id, command);
}

Target::ReturnCode ProcessTarget::sendCustom(quint64 id, const QString& custom, const QByteArray& payload)
{
    ensureStarted();
    return _target->sendCustom(id, custom, payload);
}

bool ProcessTarget::setPassword(const QString &password)
{
    ensureStarted();
    return _target->setPassword(password);
}

void ProcessTarget::clearPassword()
{
    ensureStarted();
    _target->clearPassword();
}

void ProcessTarget::ensureStarted() const
{
    if(!ProcessManager::isRunDetectionSupported()) return;
    
    bool running = false;
    Q_FOREACH(const QString &executablePath, _executablePaths) {
        const QString name = QFileInfo(executablePath).fileName();
        running |= ProcessManager::isRunning(name);
    }
    if(running) return;
    
    Q_FOREACH(const QString &executablePath, _executablePaths) {
        if(ProcessManager::start(executablePath)) break;
    }
}