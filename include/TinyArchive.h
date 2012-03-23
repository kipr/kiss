#ifndef _TINYARCHIVE_H_
#define _TINYARCHIVE_H_

#include <deque>
#include <iostream>

#define TINY_ARCHIVE_VERSION_MAJOR 1
#define TINY_ARCHIVE_VERSION_MINOR 0

struct ArchiveEntry {
	ArchiveEntry(const ArchiveEntry& other);
	ArchiveEntry(std::string path, const char* data, const size_t& length);
	~ArchiveEntry();
	
	std::string path;
	
	char* data;
	size_t length;
};

class TinyArchive : public std::deque<ArchiveEntry*>
{
public:
	~TinyArchive();
	void write(std::ostream& out) const;
	bool remove(std::string path);
	int indexOf(std::string path) const;
	bool update(std::string path, const char* data, const size_t& length);
	static TinyArchive* read(std::istream& in);
};

#endif
