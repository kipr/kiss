#ifndef _COMMUNICATION_MANAGER_HPP_
#define _COMMUNICATION_MANAGER_HPP_

#include <QObject>
#include <QRunnable>
#include <QMutex>

#include "singleton.hpp"
#include "communication_queue.hpp"

namespace kiss
{
	namespace target
	{
		class CommunicationWorker : public QObject, public QRunnable
		{
		Q_OBJECT
		public:
			CommunicationWorker(const CommunicationEntryPtr &entry);
			virtual void run();
			
		signals:
			void progress(const kiss::target::CommunicationEntryPtr &entry);
			void finished(const kiss::target::CommunicationEntryPtr &entry, const kiss::target::Target::ReturnCode success);
			
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
			void admitted(const kiss::target::CommunicationEntryPtr &entry);
			void began(const kiss::target::CommunicationEntryPtr &entry);
			void progress(const kiss::target::CommunicationEntryPtr &entry, double fraction);
			void finished(const kiss::target::CommunicationEntryPtr &entry, kiss::target::Target::ReturnCode success);
			void queueFinished();
			
			void targetNeedsAuthentication(const kiss::target::TargetPtr &target,
				kiss::target::CommunicationManager *manager);
			void oldDeviceSoftware(const kiss::target::TargetPtr &target);
			void oldHostSoftware(const kiss::target::TargetPtr &target);
			
		private slots:
			void saturate();
			void workerFinished(kiss::target::CommunicationEntryPtr entry, const kiss::target::Target::ReturnCode success);
			
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
