#ifndef _COMMUNICATION_QUEUE_MODEL_HPP_
#define _COMMUNICATION_QUEUE_MODEL_HPP_

#include "communication_queue.hpp"

#include <QStandardItemModel>

namespace Kiss
{
	namespace Target
	{
		class CommunicationManager;
		
		class CommunicationQueueModel : public QStandardItemModel
		{
		Q_OBJECT
		public:
			CommunicationQueueModel(CommunicationManager *manager, QObject *parent);
			~CommunicationQueueModel();
			
			CommunicationManager *manager() const;
			
		private slots:
			void admitted(const CommunicationEntryPtr &entry);
			void began(const CommunicationEntryPtr &entry);
			void progress(const CommunicationEntryPtr &entry, double success);
			void finished(const CommunicationEntryPtr &entry, bool success);
			void queueFinished();
			
		private:
			CommunicationManager *m_manager;
		};
	}
}

#endif
