#ifndef _COMMUNICATION_MANAGER_HPP_
#define _COMMUNICATION_MANAGER_HPP_

#include <QObject>
#include <QRunnable>
#include <QMutex>

#include "singleton.hpp"
#include "communication_queue.hpp"

namespace Kiss
{
	namespace Target
	{
		class CommunicationWorker : public QObject, public QRunnable
		{
		Q_OBJECT
		public:
			CommunicationWorker(const CommunicationEntryPtr &entry);
			virtual void run();
			
		signals:
			void progress(const CommunicationEntryPtr &entry);
			void finished(const CommunicationEntryPtr &entry, const bool &success);
			
		private:
			CommunicationEntryPtr m_entry;
		};
		
		class CommunicationManager : public QObject, public Singleton<CommunicationManager>
		{
		Q_OBJECT
		public:
			CommunicationManager();
			~CommunicationManager();
			
			bool isIdle() const;
			
			quint64 admit(const CommunicationEntryPtr &entry);
			
		signals:
			void admitted(const CommunicationEntryPtr &entry);
			void began(const CommunicationEntryPtr &entry);
			void progress(const CommunicationEntryPtr &entry, double fraction);
			void finished(const CommunicationEntryPtr &entry, bool success);
			void queueFinished();
			
		private slots:
			void saturate();
			void workerFinished(CommunicationEntryPtr entry, const bool &success);
			
		private:
			quint64 m_id;
			QList<CommunicationEntryPtr> m_running;
			CommunicationQueue m_queue;
			mutable QMutex m_queueMutex;
		};
	}
}

#endif
