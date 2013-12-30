#ifndef _PROCESS_TARGET_HPP_
#define _PROCESS_TARGET_HPP_

#include "target.hpp"

namespace kiss
{
    namespace target
    {
        class ProcessTarget : public Target
        {
        public:
            ProcessTarget(const TargetPtr &target, const QStringList &executablePaths, const QStringList &args = QStringList());
            
            virtual QMap<QString, QString> information() const; // Needs to be cached

            virtual bool disconnect();

            virtual bool available();

            virtual Target::ReturnCode compile(quint64 id, const QString& name);
            virtual Target::ReturnCode download(quint64 id, const QString& name, const kiss::KarPtr& archive);
            virtual Target::ReturnCode run(quint64 id, const QString& name);

            virtual Target::ReturnCode list(quint64 id);
            virtual Target::ReturnCode deleteProgram(quint64 id, const QString& name);
            virtual Target::ReturnCode interaction(quint64 id, const QString& command);

            virtual Target::ReturnCode sendCustom(quint64 id, const QString& custom, const QByteArray& payload = QByteArray());

            virtual bool setPassword(const QString &password);
            virtual void clearPassword();
            
            virtual void setResponder(Responder *responder);
            virtual Responder *responder() const;
            
        private:
            void ensureStarted() const;
            
            TargetPtr _target;
            QStringList _executablePaths;
            QStringList _args;
        };
    }
}

#endif
