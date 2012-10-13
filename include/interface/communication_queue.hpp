#ifndef _COMMUNICATIONQUEUE_H_
#define _COMMUNICATIONQUEUE_H_

#include <kar.hpp>

#include <QQueue>
#include <QString>
#include <QByteArray>
#include <QSharedPointer>

namespace Kiss
{
	namespace Target
	{
		class CommunicationEntry
		{
		public:
			enum Type {
				Unknown = 0,
				Download,
				Compile,
				Run,
				List,
				Delete,
				Interaction,
				Authenticate,
				Disconnect,
				Custom
			};

			CommunicationEntry(const Type& type);
			CommunicationEntry(const Type& type, const QString& name, const KarPtr& archive = KarPtr());
			CommunicationEntry(const QString& custom, const QByteArray& payload = QByteArray());

			const CommunicationEntry::Type& type() const;
			const QString& custom() const;
			const QByteArray& payload() const;
			const QString& name() const;
			
			const KarPtr& archive() const;
			
		private:
			Q_DISABLE_COPY(CommunicationEntry)
			
			Type m_type;
			QString m_name;
			QString m_custom;
			QByteArray m_payload;
			KarPtr m_archive;
		};

		typedef QSharedPointer<CommunicationEntry> CommunicationEntryPtr;
		typedef QQueue<CommunicationEntryPtr> CommunicationQueue;
	}
}

#endif
