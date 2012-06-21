#include "Socket.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
bool Socket::m_inited = false;
#endif

Address::Address() {}

Address::Address(const struct sockaddr_in& address) : m_address(address)
{
	
}

Address::Address(const char *ip, const unsigned short& port)
{
	memset(&m_address, 0, sizeof(m_address));
	m_address.sin_family = AF_INET;
	m_address.sin_addr.s_addr = inet_addr(ip);
	m_address.sin_port = htons(port);
}

Address::Address(const unsigned long& addr, const unsigned short& port)
{
	memset(&m_address, 0, sizeof(m_address));
	m_address.sin_family = AF_INET;
	m_address.sin_addr.s_addr = htonl(addr);
	m_address.sin_port = htons(port);
}

const struct sockaddr_in& Address::address() const
{
	return m_address;
}

Socket::Socket()
{
#ifdef _WIN32
	init();
	if(!m_inited) return;
#endif

	/* create what looks like an ordinary UDP socket */
	if ((m_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket");
#ifdef _WIN32
		fprintf(stderr, "win32 Error Code %d\n", WSAGetLastError());
#endif
	}
#ifndef _WIN32
	m_flags = fcntl(m_fd, F_GETFL);
#endif
}

Socket::~Socket()
{
	if(m_fd < 0) return;
	
#ifndef _WIN32
	close(m_fd);
#else
	closesocket(m_fd);
#endif
}

#ifdef _WIN32
void Socket::init()
{
	if(m_inited) return;
	
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsaData;
	int err = 0;
	if((err = WSAStartup(wVersionRequested, &wsaData)) != 0) {
		printf("WSAStartup failed with err %d\n", err);
		return;
	}
	m_inited = true;
}
#endif

bool Socket::sendto(const char* data, const size_t& size, const Address& to)
{
	if(m_fd < 0) return false;
	
	struct sockaddr_in addr = to.address();
	if(::sendto(m_fd, data, size, 0, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("sendto");
		return -1;
	}
	return 0;
}

bool Socket::recvfrom(char*& data, size_t& size, Address& from)
{
	if(m_fd < 0) return false;
	
	struct sockaddr_in addr;
	ssize_t nbytes = 0;
#ifndef _WIN32
	unsigned int addrlen = sizeof(addr);
#else
	int addrlen = sizeof(addr);
#endif
	char buffer[512];
	if((nbytes = ::recvfrom(m_fd, buffer, 512, 0, (struct sockaddr *)&addr, &addrlen)) <= 0) {
		// perror("recvfrom");
		return false;
	}
	size = nbytes;
	data = (char *)malloc(size * sizeof(char));
	memcpy(data, buffer, size);
	from = Address(addr);
	return true;
}

bool Socket::bind(const Address& address)
{
	if(m_fd < 0) return false;
	
	struct sockaddr_in addr = address.address();
	if (::bind(m_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("bind");
		return false;
	}
	return true;
}

void Socket::setBlocking(bool blocking)
{
	if(blocking) {
#ifndef _WIN32
		fcntl(m_fd, F_SETFL, m_flags);
#else
		u_long iMode = 0;
		ioctlsocket(m_fd, FIONBIO, &iMode);
#endif
	} else {
#ifndef _WIN32
		fcntl(m_fd, F_SETFL, m_flags | O_NONBLOCK);
#else
		u_long iMode = 1;
		ioctlsocket(m_fd, FIONBIO, &iMode);
#endif
	}
}

bool Socket::setReusable(bool reusable)
{
#ifndef _WIN32
	const unsigned int yes = reusable ? 1 : 0;
#else
	const char yes = reusable ? 1 : 0;
#endif
	if (setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
		perror("Reusing ADDR failed");
		return false;
	}
	return true;
}

bool Socket::joinMulticastGroup(const Address& address)
{
	struct ip_mreq mreq;
	mreq.imr_multiaddr.s_addr = address.address().sin_addr.s_addr;
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	
	if(setsockopt(m_fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (const char *)&mreq, sizeof(mreq)) < 0) {
		perror("setsockopt");
		return false;
	}
	return true;
}