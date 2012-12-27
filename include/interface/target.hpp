#ifndef _TARGET_H_
#define _TARGET_H_

#include "communication_queue.hpp"
#include "target_responder.hpp"

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
		
		class Target
		{
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
			virtual const bool compile(const QString& name) = 0;
			virtual const bool download(const QString& name, const KarPtr& archive) = 0;
			virtual const bool run(const QString& name) = 0;

			virtual const bool list() = 0;
			virtual const bool deleteProgram(const QString& name) = 0;
			virtual const bool interaction(const QString& command) = 0;

			virtual const bool authenticate(const QByteArray& hash) = 0;
			virtual const bool sendCustom(const QString& custom, const QByteArray& payload = QByteArray()) = 0;

			void addResponder(Responder *responder);
			void addResponders(const ResponderPtrList& responders);
			void removeResponder(Responder *responder);
			void clearResponders();
			const ResponderPtrList& responders() const;
			
		protected:
			void notifyQueue(const bool success);

		private:
			const bool executeEntry(const CommunicationEntry *entry);
			void clearEntries();

			CommunicationQueue m_workingQueue;
			CommunicationQueue m_queue;

			Interface *m_interface;
			ResponderPtrList m_responders;
		};

		typedef QSharedPointer<Target> TargetPtr;
	}
}

#endif
