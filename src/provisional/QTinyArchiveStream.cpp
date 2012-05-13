#include "QTinyArchiveStream.h"

QTinyArchiveStream::QTinyArchiveStream(QIODevice* device) : m_device(device)
{
	
}

bool QTinyArchiveStream::write(const TinyArchive* archive) const
{
	iostream& o = *m_stream;
	static const uint32_t magic = TINYARCHIVE_MAGIC;
	o.write(reinterpret_cast<const char*>(&magic), sizeof(uint32_t));
	uint32_t len = 0;
	char* data = archive->root()->serialize(len);
	if(!data) return false;
	o.write(reinterpret_cast<const char*>(&len), sizeof(uint32_t));
	o.write(data, len);
	delete[] data;
	return true;
}

TinyNode* QTinyArchiveStream::deserializeNode() const
{
	uint32_t len = 0;
	i.read(reinterpret_cast<char*>(&len), sizeof(uint32_t));
	if(len == 0) return 0;
	char* data = new char[len];
	i.read(data, len);
	TinyNode* ret = TinyNode::deserialize(data, len);
	delete[] data;
	return ret;
}

TinyArchive* QTinyArchiveStream::read() const
{
	iostream& i = *m_stream;
	uint32_t readMagic = 0;
	i.read(reinterpret_cast<char*>(&readMagic), sizeof(uint32_t));
	if(readMagic != TINYARCHIVE_MAGIC) return 0;
	
	std::map<uint32_t, TinyNode*> nodes;
	
	TinyNode* root = deserializeNode();
	if(!root) return 0;
	
	TinyArchive* archive = new TinyArchive(*root);
	delete root;
	return archive;
}