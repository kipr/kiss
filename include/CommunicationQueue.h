#ifndef _COMMUNICATIONQUEUE_H_
#define _COMMUNICATIONQUEUE_H_

#include <QQueue>
#include <QString>
#include <QByteArray>

class TinyArchive;

class CommunicationEntry
{
public:
	enum Type {
		Unknown = 0,
		Download,
		Compile,
		Run,
		Authenticate,
		Custom
	};
	
	CommunicationEntry(const Type& type, const QString& name, TinyArchive *archive = 0);
	CommunicationEntry(const QString& custom, const QByteArray& payload = QByteArray());
	
	const CommunicationEntry::Type& type() const;
	const QString& custom() const;
	const QByteArray& payload() const;
	const QString& name() const;
	TinyArchive *archive() const;
private:
	CommunicationEntry(const CommunicationEntry& rhs);
	CommunicationEntry& operator=(const CommunicationEntry& rhs);
	
	Type m_type;
	QString m_name;
	QString m_custom;
	QByteArray m_payload;
	TinyArchive *m_archive;
};

typedef QQueue<CommunicationEntry *> CommunicationQueue;

#endif
