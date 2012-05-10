#ifndef _LOGSTREAMBUF_H_
#define _LOGSTREAMBUF_H_

#include <streambuf>
#include <vector>

class LogWindow;

class LogStreamBuf : public std::streambuf
{
public:
	explicit LogStreamBuf(std::size_t bufferSize = 256);
protected:
	bool flush();
private:
	int_type overflow(int_type ch);
	int sync();
	
	LogStreamBuf(const LogStreamBuf&);
	LogStreamBuf& operator= (const LogStreamBuf&);
private:
	std::vector<char> m_buffer;
};

#endif
