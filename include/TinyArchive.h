#ifndef _TINYARCHIVE_H_
#define _TINYARCHIVE_H_

#include <deque>
#include <iostream>

#define TINY_ARCHIVE_VERSION_MAJOR 1
#define TINY_ARCHIVE_VERSION_MINOR 0

struct ArchiveEntry {
	ArchiveEntry(const ArchiveEntry& other);
	ArchiveEntry(std::string path, const char* data, size_t length);
	~ArchiveEntry();
	
	void setData(const char* data, size_t length);
	
	std::string path;
	
	char* data;
	size_t length;
};

class TinyArchive;

struct TinyVirtualNode
{
	const ArchiveEntry* entry() const;
	const std::deque<TinyVirtualNode*>& children() const;
	
	friend class TinyArchive;
private:
	ArchiveEntry* m_entry;
	std::deque<TinyVirtualNode> m_children;
};

class TinyArchive : public std::deque<ArchiveEntry*>
{
public:
	~TinyArchive();
	void write(std::ostream& out) const;
	bool remove(const std::string& path);
	int indexOf(const std::string& path) const;
	bool update(const std::string& path, const char* data, const size_t& length);
	static TinyArchive* read(std::istream& in);
	
	TinyVirtualNode* tree() const; // Computationally intensive operation
	
	void dump() const;
private:
	TinyVirtualNode* directory(const std::string& dir, const std::deque<ArchiveEntry*>& remaining) const;
};

#endif
