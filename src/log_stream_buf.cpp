#include "log_stream_buf.hpp"
#include "log_window.hpp"

#include <cassert>

using namespace Kiss;

LogStreamBuf::LogStreamBuf(const std::size_t& bufferSize)
	: m_buffer(bufferSize + 1)
{
	char* base = &m_buffer.front();
	setp(base, base + m_buffer.size() - 1);
}

bool LogStreamBuf::flush()
{
	std::ptrdiff_t n = pptr() - pbase();
	pbump(-n);
	char* str = new char[n + 1];
	memcpy(str, pbase(), n);
	str[n] = 0;
	Widget::LogWindow::ref().append(str);
	delete str;
	return true;
}

LogStreamBuf::int_type LogStreamBuf::overflow(int_type ch)
{
	if(ch != traits_type::eof()) {
		assert(std::less_equal<char*>()(pptr(), epptr()));
		*pptr() = ch;
		pbump(1);
		flush();
		return ch;
	}
	return traits_type::eof();
}

int LogStreamBuf::sync()
{
	return flush() ? 0 : -1;
}