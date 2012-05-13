#ifndef _ARCHIVEWRITER_H_
#define _ARCHIVEWRITER_H_

#include <QDir>
#include "DeltaArchiveListener.h"

class TinyArchive;

class ArchiveWriter
{
public:
	ArchiveWriter(TinyArchive* archive, const QDir& root);
	~ArchiveWriter();
	
	QStringList files() const;
	
	const QDir& root() const;
	const TinyArchive* archive() const;
	
	const bool write(const int& mode = Delta);
	
	enum Mode {
		Delta,
		Full
	};
private:
	const bool writeFull(const TinyNode* node);
	const bool writeDelta();
	
	const bool deltaAddEvent(const ArchiveEvent& event);
	const bool deltaUpdateEvent(const ArchiveEvent& event);
	const bool deltaRemoveEvent(const ArchiveEvent& event);
	
	TinyArchive* m_archive;
	DeltaArchiveListener* m_listener;
	QDir m_root;
};

#endif
