#include "QTinyArchiveStream.h"

#include <QIODevice>

QTinyArchiveStream::QTinyArchiveStream(QIODevice* device) : m_device(device)
{
	
}

bool QTinyArchiveStream::write(const TinyArchive* archive) const
{
	static const uint32_t magic = TINYARCHIVE_MAGIC;
	m_device->write(reinterpret_cast<const char*>(&magic), sizeof(uint32_t));
	uint32_t len = 0;
	char* data = archive->root()->serialize(len);
	if(!data) return false;
	m_device->write(reinterpret_cast<const char*>(&len), sizeof(uint32_t));
	m_device->write(data, len);
	delete[] data;
	return true;
}

TinyNode* QTinyArchiveStream::deserializeNode() const
{
	uint32_t len = 0;
	m_device->read(reinterpret_cast<char*>(&len), sizeof(uint32_t));
	if(len == 0) return 0;
	char* data = new char[len];
	m_device->read(data, len);
	TinyNode* ret = TinyNode::deserialize(data, len);
	delete[] data;
	return ret;
}

TinyArchive* QTinyArchiveStream::read() const
{
	uint32_t readMagic = 0;
	m_device->read(reinterpret_cast<char*>(&readMagic), sizeof(uint32_t));
	if(readMagic != TINYARCHIVE_MAGIC) return 0;
	
	std::map<uint32_t, TinyNode*> nodes;
	
	TinyNode* root = deserializeNode();
	if(!root) return 0;
	
	TinyArchive* archive = new TinyArchive(*root);
	delete root;
	return archive;
}