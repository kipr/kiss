#include "process_target.hpp"
#include "process_manager.hpp"

#include <QFileInfo>
#include <QWaitCondition>
#include <QMutex>
#include <QDebug>

using namespace kiss;
using namespace kiss::target;

#define ON_ERROR_STARTUP(x) \
  Target::ReturnCode ret = Target::Error; \
  if((ret = x) == Target::Error) { \
    ensureStarted(); \
    ret = x; \
  } \
  return ret

ProcessTarget::ProcessTarget(const TargetPtr &target, const QList<QString> &executablePaths)
  : Target(target->interface(), "process_target")
  , _target(target)
  , _executablePaths(executablePaths)
{
}

QMap<QString, QString> ProcessTarget::information() const
{
  return _target->information();
}

bool ProcessTarget::disconnect()
{
  return _target->disconnect();
}

bool ProcessTarget::available()
{
  return _target->available();
}

Target::ReturnCode ProcessTarget::compile(quint64 id, const QString& name)
{
  ON_ERROR_STARTUP(_target->compile(id, name));
}

Target::ReturnCode ProcessTarget::download(quint64 id, const QString& name, const kiss::KarPtr& archive)
{
  ON_ERROR_STARTUP(_target->download(id, name, archive));
}

Target::ReturnCode ProcessTarget::run(quint64 id, const QString& name)
{
  ON_ERROR_STARTUP(_target->run(id, name));
}

Target::ReturnCode ProcessTarget::list(quint64 id)
{
  ON_ERROR_STARTUP(_target->list(id));
}

Target::ReturnCode ProcessTarget::deleteProgram(quint64 id, const QString& name)
{
  ON_ERROR_STARTUP(_target->deleteProgram(id, name));
}

Target::ReturnCode ProcessTarget::interaction(quint64 id, const QString& command)
{
  ON_ERROR_STARTUP(_target->interaction(id, command));
}

Target::ReturnCode ProcessTarget::sendCustom(quint64 id, const QString& custom, const QByteArray& payload)
{
  ON_ERROR_STARTUP(_target->sendCustom(id, custom, payload));
}

bool ProcessTarget::setPassword(const QString &password)
{
  return _target->setPassword(password);
}

void ProcessTarget::clearPassword()
{
  _target->clearPassword();
}

void ProcessTarget::setResponder(Responder *responder)
{
  _target->setResponder(responder);
}

Responder *ProcessTarget::responder() const
{
  return _target->responder();
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
    qDebug() << "Trying to start" << executablePath;
    if(ProcessManager::start(executablePath)) break;
  }
  
  QWaitCondition waitCondition;
  QMutex mutex;

  waitCondition.wait(&mutex, 3000);
}