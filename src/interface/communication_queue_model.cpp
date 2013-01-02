#include "communication_queue_model.hpp"

#include "communication_manager.hpp"

#include <QStandardItem>

namespace Kiss
{
	namespace Target
	{
		class CommunicationItem : public QStandardItem
		{
		public:
			CommunicationItem(const CommunicationEntryPtr &entry)
				: QStandardItem(entry->typeString()),
				m_entry(entry)
			{
				setSizeHint(QSize(100, 20));
			}

			const CommunicationEntryPtr &entry()
			{
				return m_entry;
			}

			template<typename T>
			static CommunicationItem *cast(T *t)
			{
				return dynamic_cast<CommunicationItem *>(t);
			}
			
		private:
			CommunicationEntryPtr m_entry;
		};
		
		class StatusItem : public QStandardItem
		{
		public:
			enum Status {
				Waiting,
				Executing
			};
			
			StatusItem()
			{
				setSizeHint(QSize(100, 20));
				setStatus(StatusItem::Waiting);
			}
			
			void setStatus(const StatusItem::Status &status)
			{
				m_status = status;
				updateText();
			}
			
			const StatusItem::Status &status()
			{
				return m_status;
			}
			
			template<typename T>
			static StatusItem *cast(T *t)
			{
				return dynamic_cast<StatusItem *>(t);
			}
			
		private:
			void updateText()
			{
				switch(m_status) {
				case StatusItem::Waiting: setText(QObject::tr("Waiting..."));
				case StatusItem::Executing: setText(QObject::tr("Executing..."));
				}
			}
			
			Status m_status;
		};
	}
}

using namespace Kiss;
using namespace Kiss::Target;

CommunicationQueueModel::CommunicationQueueModel(CommunicationManager *manager, QObject *parent)
	: QStandardItemModel(parent),
	m_manager(manager)
{
	connect(m_manager, SIGNAL(admitted(CommunicationEntryPtr)),
		SLOT(admitted(CommunicationEntryPtr)));
	connect(m_manager, SIGNAL(began(CommunicationEntryPtr)),
		SLOT(began(CommunicationEntryPtr)));
	connect(m_manager, SIGNAL(progress(CommunicationEntryPtr, double)),
		SLOT(progress(CommunicationEntryPtr, double)));
	connect(m_manager, SIGNAL(finished(CommunicationEntryPtr, bool)),
		SLOT(finished(CommunicationEntryPtr, bool)));
	connect(m_manager, SIGNAL(queueFinished()), SLOT(queueFinished()));
}

CommunicationQueueModel::~CommunicationQueueModel()
{
	
}

CommunicationManager *CommunicationQueueModel::manager() const
{
	return m_manager;
}

void CommunicationQueueModel::admitted(const CommunicationEntryPtr &entry)
{
	appendRow(QList<QStandardItem *>()
		<< new CommunicationItem(entry)
		<< new StatusItem());
}

void CommunicationQueueModel::began(const CommunicationEntryPtr &entry)
{
	for(int i = 0; i < rowCount(); ++i) {
		CommunicationItem *commItem = CommunicationItem::cast(item(i, 0));
		if(!commItem || commItem->entry()->id() != entry->id()) continue;
		StatusItem *statItem = StatusItem::cast(item(i, 1));
		statItem->setStatus(StatusItem::Executing);
	}
}

void CommunicationQueueModel::progress(const CommunicationEntryPtr &entry, double fraction)
{

}

void CommunicationQueueModel::finished(const CommunicationEntryPtr &entry, bool success)
{
	for(int i = 0; i < rowCount(); ++i) {
		CommunicationItem *commItem = CommunicationItem::cast(item(i));
		if(!commItem) continue;
		if(commItem->entry()->id() == entry->id()) qDeleteAll(takeRow(i));
	}
}

void CommunicationQueueModel::queueFinished()
{
	clear();
}