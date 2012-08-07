#include "SerialInterface.h"
#include "SerialDevice.h"

#ifndef Q_OS_WIN32
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#else
#include <windows.h>
#endif

SerialInterface::SerialInterface()
	: Interface("Serial")
{
	
}

SerialInterface::~SerialInterface()
{
	
}

const bool SerialInterface::scan(InterfaceResponder *responder)
{
	m_responder = responder;
	
	m_responder->targetScanStarted(this);
	
	QStringList portList;

#ifdef Q_OS_WIN32
	// Sets the timeouts so that operations are non-blocking
	COMMTIMEOUTS cto;
	cto.ReadIntervalTimeout = MAXDWORD;
	cto.ReadTotalTimeoutMultiplier = 0;
	cto.ReadTotalTimeoutConstant = 0;
	cto.WriteTotalTimeoutMultiplier = 0;
	cto.WriteTotalTimeoutConstant = 0;


	// Scan through ports 1-32	
	for(int i = 2; i <= 32; ++i) {
		QString portName = "com" + QString::number(i);
		QString realPortName = "\\\\.\\com" + QString::number(i);
		
		HANDLE handle = CreateFileA(realPortName.toLocal8Bit(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, 0);
		
		// Add the port to the list of available names if it's valid
		if(handle != INVALID_HANDLE_VALUE) {
			CloseHandle(handle);
			portList << portName;
		}
	}
	
#else

	QDir devDirectory("/dev");
	QStringList devListFilter;

#ifdef Q_OS_MAC
	devListFilter << "tty.usb*";
#else
	devListFilter << "ttyUSB*";
#endif

	QFileInfoList potentialPortList = devDirectory.entryInfoList(devListFilter, QDir::System | QDir::CaseSensitive);

	//Check through the returned ports to see which ones might be valid
	QListIterator<QFileInfo> i(potentialPortList);
	while(i.hasNext()) {
		QFileInfo current = i.next();
		int stream = ::open(current.absoluteFilePath().toLocal8Bit(), O_RDWR | O_NDELAY);
		struct termios tio;
		// If I can get serial attributes then the port is good
		if(tcgetattr(stream, &tio) != -1) portList << current.absoluteFilePath();
		::close(stream);
	}
#endif
	foreach(const QString& port, portList) m_responder->targetFound(this, TargetPtr(new SerialDevice(this, port)));
	return true;
}

void SerialInterface::invalidateResponder()
{
	m_responder = 0;
}

Q_EXPORT_PLUGIN2(serial_interface, SerialInterface);