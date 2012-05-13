#ifndef _DEVICE_H_
#define _DEVICE_H_

#include "Named.h"
#include "QTinyArchive.h"

#include <QString>
#include <QMap>
#include <boost/shared_ptr.hpp>

class Project;

#define NOT_AVAILABLE "N/A"

#define DISPLAY_NAME "disp_name"
#define SERIAL "serial"

#define DOWNLOAD_TYPE "download_type"
#define DOWNLOAD_SRC "src"
#define DOWNLOAD_BIN "bin"

class Device : public Named
{
public:
	Device(const QString& name);
	virtual ~Device();
	
	virtual const QMap<QString, QString> information() = 0;
	virtual const bool available() = 0;
	virtual const bool download(const QString& name, TinyArchive* archive) = 0;
	virtual const bool run(const QString& name) = 0;
};

typedef boost::shared_ptr<Device> DevicePtr;


#endif
