#ifndef _DEVICE_H_
#define _DEVICE_H_

#include "Named.h"
#include "QTinyArchive.h"
#include <kiss-compiler/Compiler.h>

#include <QString>
#include <QMap>
#include <boost/shared_ptr.hpp>

#define NOT_AVAILABLE "N/A"

#define DISPLAY_NAME "disp_name"
#define DEVICE_TYPE "device_type"
#define SERIAL "serial"

#define DOWNLOAD_TYPE "download_type"
#define DOWNLOAD_SOURCE "src"
#define DOWNLOAD_BINARY "bin"

class Project;
class Interface;

class Device : public Named
{
public:
	Device(Interface* interface, const QString& name);
	virtual ~Device();
	
	Interface* interface() const;
	
	const QString displayName() const;
	const QString type() const;
	const QString serial() const;
	const int downloadType() const;
	
	enum {
		Source,
		Binary
	};
	
	virtual const QMap<QString, QString> information() const = 0; // Needs to be cached
	
	virtual const bool available() = 0;
	virtual const CompileResult compile(const QString& name) = 0;
	virtual const bool download(const QString& name, TinyArchive* archive) = 0;
	virtual const bool run(const QString& name) = 0;
	
	
private:
	Interface* m_interface;
};

typedef boost::shared_ptr<Device> DevicePtr;


#endif
