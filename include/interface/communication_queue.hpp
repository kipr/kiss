#ifndef _COMMUNICATIONQUEUE_H_
#define _COMMUNICATIONQUEUE_H_

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
			CommunicationEntry(const Type& type, const QString& name);
			CommunicationEntry(const QString& custom, const QByteArray& payload = QByteArray());

			const CommunicationEntry::Type& type() const;
			const QString& custom() const;
			const QByteArray& payload() const;
			const QString& name() const;
			
			// TinyArchive *archive() const;
		private:
			CommunicationEntry(const CommunicationEntry& rhs);
			CommunicationEntry& operator=(const CommunicationEntry& rhs);

			Type m_type;
			QString m_name;
			QString m_custom;
			QByteArray m_payload;
		};

		typedef QSharedPointer<CommunicationEntry> CommunicationEntryPtr;
		typedef QQueue<CommunicationEntryPtr> CommunicationQueue;
	}
}

#endif
