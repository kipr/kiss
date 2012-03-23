#include "TinyArchive.h"

#include <cstring>

static const char g_tinyArchiveMagic[2] = { 0x83, 0x74 };

ArchiveEntry::ArchiveEntry(const ArchiveEntry& other)
{
	path = other.path;
	
	length = other.length;
	data = new char[length];
	memcpy(data, other.data, length);
}

ArchiveEntry::ArchiveEntry(std::string path, const char* data, const size_t& length)
	: path(path), data(0), length(length)
{
	this->data = new char[length];
	memset(this->data, 0, length);
	memcpy(this->data, data, length);
}

ArchiveEntry::~ArchiveEntry()
{
	delete[] data;
}

TinyArchive::~TinyArchive()
{
	std::deque<ArchiveEntry*>::const_iterator it = begin();
	for(; it != end(); ++it) delete *it;
}

void TinyArchive::write(std::ostream& out) const
{
	out.write(g_tinyArchiveMagic, sizeof(char) * 2);
	
	char majorVersion = TINY_ARCHIVE_VERSION_MAJOR;
	char minorVersion = TINY_ARCHIVE_VERSION_MINOR;
	out.write(&majorVersion, sizeof(char));
	out.write(&minorVersion, sizeof(char));
	
	size_t numEntries = size();
	out.write((char*)&numEntries, sizeof(size_t));
	std::deque<ArchiveEntry*>::const_iterator it = begin();
	for(; it != end(); ++it) {
		const ArchiveEntry* entry = *it;
		size_t pathLength = entry->path.size();
		out.write((char*)&pathLength, sizeof(size_t));
		out.write(entry->path.c_str(), pathLength);
		out.write((char*)&entry->length, sizeof(size_t));
		out.write(entry->data, entry->length);
	}
}

bool TinyArchive::remove(std::string path)
{
	std::deque<ArchiveEntry*>::iterator it = begin();
	for(; it != end(); ++it) {
		if(!path.compare((*it)->path)) {
			it = erase(it);
			return true;
		}
	}
	return false;
}

int TinyArchive::indexOf(std::string path) const
{
	std::deque<ArchiveEntry*>::const_iterator it = begin();
	int i = 0;
	for(; it != end(); ++it) {
		if(!path.compare((*it)->path)) return i;
		++i;
	}
	
	return -1;
}

bool TinyArchive::update(std::string path, const char* data, const size_t& length)
{
	std::deque<ArchiveEntry*>::iterator it = begin();
	for(; it != end(); ++it) {
		if(!path.compare((*it)->path)) {
			delete[] (*it)->data;
			(*it)->data = new char[length];
			memset((*it)->data, 0, length);
			memcpy((*it)->data, data, length);
			return true;
		}
	}
	
	std::cout << "Updated failed for " << path << std::endl;
	
	return false;
}

TinyArchive* TinyArchive::read(std::istream& in)
{
	char magic[2];
	in.read(magic, sizeof(char) * 2);
	
	char majorVersion = 0;
	in.read(&majorVersion, sizeof(char));
	if(majorVersion != TINY_ARCHIVE_VERSION_MAJOR) return 0;
	
	char minorVersion = 0;
	in.read(&minorVersion, sizeof(char));
	if(minorVersion != TINY_ARCHIVE_VERSION_MINOR) return 0;
	
	std::deque<ArchiveEntry*> entries;
	
	size_t numEntries = 0;
	in.read((char*)&numEntries, sizeof(size_t));
	
	for(size_t i = 0; i < numEntries; ++i) {
		size_t pathLength = 0;
		in.read((char*)&pathLength, sizeof(size_t));
		
		char* path = new char[pathLength + 1];
		in.read(path, pathLength);
		path[pathLength] = 0;
		
		size_t dataLength = 0;
		in.read((char*)&dataLength, sizeof(size_t));
		
		char* data = new char[dataLength];
		in.read(data, dataLength);
		
		entries.push_back(new ArchiveEntry(path, data, dataLength));
		
		delete[] path;
		delete[] data;
	}
	
	TinyArchive* ret = new TinyArchive();
	ret->assign(entries.begin(), entries.end());
	return ret;
}