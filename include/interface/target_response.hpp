#ifndef _TARGET_RESPONSE_HPP_
#define _TARGET_RESPONSE_HPP_

#include <QString>
#include <QVariant>
#include <QMetaType>

namespace Kiss
{
	namespace Target
	{
		class Response
		{
		public:
			Response();
			Response(quint64 id, const QString &type, const QVariant &data = QVariant());
			
			const quint64 &id() const;
			const QString &type() const;
			const QVariant &data() const;
			
		private:
			quint64 m_id;
			QString m_type;
			QVariant m_data;
		};
	}
}

Q_DECLARE_METATYPE(Kiss::Target::Response);

#endif
