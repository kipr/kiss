#ifndef _COMMUNICATION_QUEUE_MODEL_HPP_
#define _COMMUNICATION_QUEUE_MODEL_HPP_

#include "communication_queue.hpp"

#include <QStandardItemModel>

namespace kiss
{
	namespace target
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
			void admitted(const kiss::target::CommunicationEntryPtr &entry);
			void began(const kiss::target::CommunicationEntryPtr &entry);
			void progress(const kiss::target::CommunicationEntryPtr &entry, double success);
			void finished(const kiss::target::CommunicationEntryPtr &entry, kiss::target::Target::ReturnCode ret);
			void queueFinished();
			
		private:
			CommunicationManager *m_manager;
		};
	}
}

#endif
