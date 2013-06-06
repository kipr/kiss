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
			void progress(const Kiss::Target::CommunicationEntryPtr &entry);
			void finished(const Kiss::Target::CommunicationEntryPtr &entry, const Kiss::Target::Target::ReturnCode success);
			
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
			
			// TODO: Pause by target
			bool isPaused() const;
			void setPaused(const bool pause);
			
			void clearQueue();
			void clearQueue(const TargetPtr &target);
			
			quint64 admit(const CommunicationEntryPtr &entry);
			
		signals:
			void admitted(const Kiss::Target::CommunicationEntryPtr &entry);
			void began(const Kiss::Target::CommunicationEntryPtr &entry);
			void progress(const Kiss::Target::CommunicationEntryPtr &entry, double fraction);
			void finished(const Kiss::Target::CommunicationEntryPtr &entry, Kiss::Target::Target::ReturnCode success);
			void queueFinished();
			
			void targetNeedsAuthentication(const Kiss::Target::TargetPtr &target,
				Kiss::Target::CommunicationManager *manager);
			void oldDeviceSoftware(const Kiss::Target::TargetPtr &target);
			void oldHostSoftware(const Kiss::Target::TargetPtr &target);
			
		private slots:
			void saturate();
			void workerFinished(Kiss::Target::CommunicationEntryPtr entry, const Kiss::Target::Target::ReturnCode success);
			
		private:
			bool m_paused;
			quint64 m_id;
			QList<CommunicationEntryPtr> m_running;
			CommunicationQueue m_queue;
			mutable QMutex m_queueMutex;
		};
	}
}

#endif
