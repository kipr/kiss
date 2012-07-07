#ifndef _DEVICE_H_
#define _DEVICE_H_

#include "Named.h"
#include "CommunicationQueue.h"
#include <TinyArchive.h>
#include <kiss-compiler/Compiler.h>

#include <QObject>
#include <QString>
#include <QMap>
#include <boost/shared_ptr.hpp>

#define NOT_AVAILABLE "N/A"

#define DISPLAY_NAME "disp_name"
#define DEVICE_TYPE "device_type"
#define COMM_PORT "comm_port"
#define SERIAL "serial"
#define VERSION "version"

#define DOWNLOAD_TYPE "download_type"
#define DOWNLOAD_SOURCE "src"
#define DOWNLOAD_BINARY "bin"

class Project;
class Interface;
class DeviceResponder;

class Device
{
public:
	Device(Interface* interface, const QString& name);
	virtual ~Device();
	
	Interface* interface() const;
	
	virtual const QString displayName() const;
	const QString type() const;
	const QString commPort() const;
	const QString serial() const;
    const QString version() const;

	
	virtual const QMap<QString, QString> information() const = 0; // Needs to be cached
	
	const bool retryLastQueue();
	const bool executeQueue(const CommunicationQueue& queue);
	const bool isQueueExecuting() const;
	
	virtual const bool disconnect() = 0;
	
	virtual const bool available() = 0;
	virtual const bool compile(const QString& name) = 0;
	virtual const bool download(const QString& name, TinyArchive* archive) = 0;
	virtual const bool run(const QString& name) = 0;
	virtual const bool authenticate(const QByteArray& hash) = 0;
	virtual const bool sendCustom(const QString& custom, const QByteArray& payload = QByteArray()) = 0;
	
	void setResponder(DeviceResponder *responder);
	DeviceResponder *responder();
protected:
	void notifyQueue(const bool success);
	
private:
	const bool executeEntry(const CommunicationEntry *entry);
	void clearEntries();
	
	CommunicationQueue m_workingQueue;
	CommunicationQueue m_queue;
	
	Interface* m_interface;
	DeviceResponder *m_responder;
};

typedef boost::shared_ptr<Device> DevicePtr;


#endif
