#include "SerialIODevice.h"

#ifndef WIN32
#include <fcntl.h>
#include <termios.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <errno.h>
#else
#include <windows.h>
#endif

#include <QDebug>

SerialIODevice::SerialIODevice(const QString& path)
	: m_path(path),
	m_baudRate(115200)
{
	qDebug() << "Created serial device at" << m_path << "with BR" << m_baudRate;
}

SerialIODevice::~SerialIODevice()
{
	close();
}

bool SerialIODevice::open(QIODevice::OpenMode mode)
{
#ifdef WIN32
	COMMTIMEOUTS cto;
	static const quint16 realPortnameSize = 16;
	char realPortname[realPortnameSize];
	memset(realPortname, 0, realPortnameSize);
	strncpy(realPortname, "\\\\.\\", 4);
	strncpy(realPortname + 4, m_path.toLocal8Bit(), 5);

	m_fd = CreateFileA(realPortname, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, 0);
	if(m_fd == INVALID_HANDLE_VALUE) return false;

	cto.ReadIntervalTimeout = 10; // This value has been set arbitrarily
	cto.ReadTotalTimeoutMultiplier = 0;
	cto.ReadTotalTimeoutConstant = 0;
	cto.WriteTotalTimeoutMultiplier = 0;
	cto.WriteTotalTimeoutConstant = 0;

	if(!SetCommTimeouts(m_fd, &cto)) {
		close();
		return false;
	}

	if(!SetupComm(m_fd, 2048, 2048)) {
		close();
		return false;
	}

	if(!SetCommMask(m_fd, 0)) {
		close();
		return false;
	}
#else
	m_fd = ::open(m_path.toLocal8Bit(), O_RDWR | O_NDELAY | O_NOCTTY);

	if(m_fd <= 0) return false;
	::fcntl(m_fd, F_SETFL, ~O_NDELAY & ::fcntl(m_fd, F_GETFL, 0));
#endif
	qDebug() << "Opening serial port";
	configurePort();
	return QIODevice::open(ReadWrite);
}

void SerialIODevice::close()
{
#ifdef WIN32
	if(m_fd == INVALID_HANDLE_VALUE) return;
	CloseHandle(m_fd);
	m_fd = INVALID_HANDLE_VALUE;
#else
	if(m_fd <= 0) return;
	::close(m_fd);
	m_fd = -1;
#endif
	qDebug() << "Closing serial port";
	QIODevice::close();
}

void SerialIODevice::configurePort()
{
#ifdef WIN32
	DCB dcb;
	memset(&dcb, 0, sizeof(dcb));
	
	dcb.DCBlength = sizeof(dcb);

	if(!GetCommState(m_fd, &dcb)) return;

	dcb.DCBlength = sizeof(dcb);
	dcb.BaudRate = m_baudRate;
	dcb.fBinary = 1;
	dcb.fOutxCtsFlow = 0;
	dcb.fOutxDsrFlow = 0;
	dcb.fDtrControl = DTR_CONTROL_DISABLE;
	dcb.fDsrSensitivity = 0;
	dcb.fTXContinueOnXoff = 0;
	dcb.fOutX = 0;
	dcb.fInX = 0;
	dcb.fErrorChar = 0;
	dcb.fNull = 0;
	dcb.fRtsControl = RTS_CONTROL_TOGGLE;
	dcb.fAbortOnError = FALSE;
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.fParity = 0;
	dcb.StopBits = ONESTOPBIT;

	SetCommState(m_fd, &dcb);
#else
	struct termios tio;

	tcgetattr(m_fd, &tio);
	
	tio.c_iflag = IGNBRK;
	tio.c_oflag = 0;
	tio.c_cflag = CREAD | CLOCAL | CS8;
	tio.c_lflag = 0;
	bzero(&tio.c_cc, sizeof(tio.c_cc));
	
	cfsetspeed(&tio, m_baudRate);

	tcsetattr(m_fd, TCSADRAIN, &tio);
#endif
	qDebug() << "Configured serial port";
}

void SerialIODevice::setBaudRate(const quint32& baudRate)
{
	m_baudRate = baudRate;
	if(isOpen()) configurePort();
}

quint32 SerialIODevice::baudRate() const
{
	return m_baudRate;
}

qint64 SerialIODevice::readData(char *data, qint64 maxSize)
{
#ifdef WIN32
	if(m_fd == INVALID_HANDLE_VALUE) return -1;
	DWORD read_count = 0;
	qWarning("ReadFile...");
	if(!ReadFile(m_fd, data, maxSize, &read_count, NULL)) {
		qWarning("Readfile failed...");
		return 0;
	}
	qWarning("Readfile returned %d", read_count);
	return read_count;
#else
	if(m_fd <= 0) return -1; 
	if(select(500)) {
		int ret = ::read(m_fd, data, maxSize);
		if(ret >= 0) return ret;
	}
	return 0;
#endif
}

qint64 SerialIODevice::writeData(const char *data, qint64 maxSize)
{
#ifdef WIN32
	if(m_fd == INVALID_HANDLE_VALUE) return -1;
	DWORD write_count;
	return !WriteFile(m_fd, data, maxSize, &write_count, NULL) ? 0 : write_count;
#else
	if(m_fd <= 0) return -1;
	int ret = ::write(m_fd, data, maxSize);
	if(ret >= 0) return ret;
	if(ret == -EAGAIN) return 0;
	return -1;
#endif
}

#ifndef WIN32
bool SerialIODevice::select(const quint32& msecs)
{
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(m_fd, &fds);

	timeval timeout;
	timeout.tv_sec  = (msecs / 1000);
	timeout.tv_usec = (msecs % 1000) * 1000;

	int n = ::select(m_fd + 1, &fds, 0, 0, &timeout);

	return n == 1;
}
#endif
