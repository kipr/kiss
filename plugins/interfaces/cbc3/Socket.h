#ifndef _SOCKET_H_
#define _SOCKET_H_

#ifndef _WIN32
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#else
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windef.h>
#endif

class Address
{
public:
	Address();
	Address(const struct sockaddr_in& address);
	Address(const char *ip, const unsigned short& port);
	Address(const unsigned long& addr, const unsigned short& port);
	
	const struct sockaddr_in& address() const;
private:
	struct sockaddr_in m_address;
};

class Socket
{
public:
	Socket();
	~Socket();
	
	bool sendto(const char* data, const size_t& size, const Address& to);
	bool recvfrom(char*& data, size_t& size, Address& from);
	
	bool bind(const Address& address);
	void setBlocking(bool blocking);
	bool setReusable(bool reusable);
	bool joinMulticastGroup(const Address& address);
private:
	int m_fd;
#ifndef _WIN32
	int m_flags;
#else
	static void init();
	static bool m_inited;
#endif
};

#endif
