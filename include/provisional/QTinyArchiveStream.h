#ifndef _QTINYARCHIVESTREAM_H_
#define _QTINYARCHIVESTREAM_H_

#include "TinyArchive.h"

class QTinyArchiveStream : public TinyArchiveReader, public TinyArchiveWriter
{
public:
	QTinyArchiveStream(QIODevice* device);
	
	virtual bool write(const TinyArchive* archive) const;
	virtual TinyArchive* read() const;
	
protected:
	virtual TinyNode* deserializeNode() const;
	
	QIODevice* m_device;
};

#endif
