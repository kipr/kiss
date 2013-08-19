#ifndef _TARGET_HPP_
#define _TARGET_HPP_

#include "target_responder.hpp"

#include <kar/kar.hpp>

#include <QObject>
#include <QString>
#include <QMap>
#include <QSharedPointer>

#define TARGET_DISPLAY_NAME "disp_name"
#define TARGET_DEVICE_TYPE "device_type"
#define TARGET_COMM_PORT "comm_port"
#define TARGET_SERIAL "serial"
#define TARGET_VERSION "version"

namespace kiss
{
	namespace project
	{
		class Project;
	}
	
	namespace target
	{
		class Interface;
		
		class Target : public QObject
		{
		Q_OBJECT
		public:
			enum ReturnCode {
				Success = 0,
				Error,
				AuthenticationFailed,
				NotImplemented,
				OldDeviceSoftware,
				OldHostSoftware
			};
			
			Target(Interface *interface, const QString &name);
			virtual ~Target();

			Interface *interface() const;

			virtual QString displayName() const;
			QString type() const;
			QString commPort() const;
			QString serial() const;
			QString version() const;

			virtual QMap<QString, QString> information() const = 0; // Needs to be cached

			virtual bool disconnect() = 0;

			virtual bool available() = 0;
			virtual ReturnCode compile(quint64 id, const QString &name) = 0;
			virtual ReturnCode download(quint64 id, const QString &name, const KarPtr &archive) = 0;
			virtual ReturnCode run(quint64 id, const QString &name) = 0;

			virtual ReturnCode list(quint64 id) = 0;
			virtual ReturnCode deleteProgram(quint64 id, const QString &name) = 0;
			virtual ReturnCode interaction(quint64 id, const QString &command) = 0;

			virtual ReturnCode sendCustom(quint64 id, const QString &custom, const QByteArray &payload = QByteArray()) = 0;
			
			virtual void setResponder(Responder *responder);
			virtual Responder *responder() const;
			
			virtual bool setPassword(const QString &password) = 0;
			virtual void clearPassword() = 0; 
			
			// This is pretty jank. Basically we use sigslots to get us back
			// on the GUI thread once an asynchronous event happens.
		signals:
			void response(const Response &response);
			
		private slots:
			void responseRedirect(const Response &response);
			
		protected:
			void notifyQueue(const bool success);

		private:

			Interface *m_interface;
			Responder *m_responder;
		};

		typedef QSharedPointer<Target> TargetPtr;
	}
}

#endif
