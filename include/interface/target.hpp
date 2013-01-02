#ifndef _TARGET_HPP_
#define _TARGET_HPP_

#include "target_responder.hpp"

#include <kar.hpp>

#include <QObject>
#include <QString>
#include <QMap>
#include <QSharedPointer>

#define NOT_AVAILABLE "N/A"

#define DISPLAY_NAME "disp_name"
#define DEVICE_TYPE "device_type"
#define COMM_PORT "comm_port"
#define SERIAL "serial"
#define VERSION "version"

#define DOWNLOAD_TYPE "download_type"
#define DOWNLOAD_SOURCE "src"
#define DOWNLOAD_BINARY "bin"

namespace Kiss
{
	namespace Project
	{
		class Project;
	}
	
	namespace Target
	{
		class Interface;
		
		class Target : public QObject
		{
		Q_OBJECT
		public:
			Target(Interface* interface, const QString& name);
			virtual ~Target();

			Interface* interface() const;

			virtual const QString displayName() const;
			const QString type() const;
			const QString commPort() const;
			const QString serial() const;
			const QString version() const;

			virtual const QMap<QString, QString> information() const = 0; // Needs to be cached

			virtual const bool disconnect() = 0;

			virtual const bool available() = 0;
			virtual const bool compile(quint64 id, const QString &name) = 0;
			virtual const bool download(quint64 id, const QString &name, const KarPtr& archive) = 0;
			virtual const bool run(quint64 id, const QString &name) = 0;

			virtual const bool list(quint64 id) = 0;
			virtual const bool deleteProgram(quint64 id, const QString& name) = 0;
			virtual const bool interaction(quint64 id, const QString& command) = 0;

			virtual const bool authenticate(quint64 id, const QByteArray& hash) = 0;
			virtual const bool sendCustom(quint64 id, const QString& custom, const QByteArray& payload = QByteArray()) = 0;
			
			void setResponder(Responder *responder);
			Responder *responder() const;
			
			
			// This is pretty jank. Basically we use sigslots to get us back
			// on the GUI thread once an asynchronous event happens.
		signals:
			void response(const Response &response);
			
		private slots:
			void responseRedirect(const Response& response);
			
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
