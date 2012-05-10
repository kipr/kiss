#ifndef _DELTAARCHIVELISTENER_H_
#define _DELTAARCHIVELISTENER_H_

#include "QTinyArchive.h"

#include <QList>
#include <QString>

struct ArchiveEvent
{
	ArchiveEvent(const int& type, const char* path, const TinyNode* node);
	~ArchiveEvent();
	ArchiveEvent(const ArchiveEvent& rhs);
	
	int type;
	const TinyNode* node;
	
	enum Type {
		NodeAdded,
		NodeRemoved,
		NodeUpdated,
		Unknown
	};
	
	const char* path() const;
private:
	void setPath(const char* path);
	char* m_path;
};

class DeltaArchiveListener : public TinyArchiveListener
{
public:
	DeltaArchiveListener(TinyArchive* archive, uint32_t id = 0);
	virtual ~DeltaArchiveListener();
	
	TinyArchive* archive() const;
	
	void drain();
	const bool hasEvent() const;
	ArchiveEvent nextEvent();
private:
	void nodeAdded(const TinyNode* node);
	void nodeRemoved(const TinyNode* node);
	void nodeUpdated(const TinyNode* node);
	
	ArchiveEvent pop();
	void push(const ArchiveEvent& event);
	
	TinyArchive* m_archive;
	uint32_t m_id;
	QList<ArchiveEvent> m_history;
};


#endif
