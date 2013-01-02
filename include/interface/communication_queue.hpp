#ifndef _COMMUNICATION_QUEUE_HPP_
#define _COMMUNICATION_QUEUE_HPP_

#include "target.hpp"

#include <kar.hpp>

#include <QQueue>
#include <QString>
#include <QByteArray>
#include <QSharedPointer>
#include <QMetaType>

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

			CommunicationEntry(const TargetPtr &target, const Type& type);
			CommunicationEntry(const TargetPtr &target, const Type& type, const QString& name, const KarPtr& archive = KarPtr());
			CommunicationEntry(const TargetPtr &target, const QString& custom, const QByteArray& payload = QByteArray());

			const TargetPtr &target() const;
			const CommunicationEntry::Type& type() const;
			QString typeString() const;
			const QString& custom() const;
			const QByteArray& payload() const;
			const QString& name() const;
			
			void setId(const quint64 &id);
			const quint64 &id() const;
			
			bool execute() const;
			
			const KarPtr& archive() const;
			
		private:
			Q_DISABLE_COPY(CommunicationEntry)
			
			TargetPtr m_target;
			Type m_type;
			QString m_name;
			QString m_custom;
			QByteArray m_payload;
			KarPtr m_archive;
			quint64 m_id;
		};

		typedef QSharedPointer<CommunicationEntry> CommunicationEntryPtr;
		typedef QQueue<CommunicationEntryPtr> CommunicationQueue;
	}
}

Q_DECLARE_METATYPE(Kiss::Target::CommunicationEntryPtr);

#endif
