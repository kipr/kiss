#ifndef _COMMUNICATION_MANAGER_HPP_
#define _COMMUNICATION_MANAGER_HPP_

#include <QObject>
#include <QRunnable>

#include "singleton.hpp"
#include "communication_queue.hpp"

namespace Kiss
{
	namespace Target
	{
		class CommunicationWorker : public QObject, public QRunnable
		{
		public:
			CommunicationWorker(const CommunicationEntryPtr &entry);
			virtual void run();
			
		signals:
			void progress(const CommunicationEntryPtr &entry);
			void finished(const CommunicationEntryPtr &entry);
			
		private:
			CommunicationEntryPtr m_entry;
		};
		
		class CommunicationManager : public QObject, public Singleton<CommunicationManager>
		{
		public:
			CommunicationManager();
			~CommunicationManager();
			
			quint64 admit(const CommunicationEntryPtr &entry);
			
		signals:
			void admitted(const CommunicationEntryPtr &entry);
			void began(const CommunicationEntryPtr &entry);
			void progress(const CommunicationEntryPtr &entry, const double &fraction);
			void finished(const CommunicationEntryPtr &entry);
			
		private slots:
			void saturate();
			void workerFinished(const CommunicationEntryPtr &entry);
			
		private:
			quint64 m_id;
			QList<CommunicationEntryPtr> m_running;
			CommunicationQueue m_queue;
		};
	}
}

#endif
