#ifndef _SERIALDEVICE_H_
#define _SERIALDEVICE_H_

#include "Target.h"
#include "SerialIODevice.h"
#include "Keys.h"
#include <easydevice/DeviceInfo.h>
#include <QtGlobal>
#include <QString>

class SerialDevice : public QObject, public Target
{
Q_OBJECT
public:
	SerialDevice(Interface *interface, const QString& path);
	~SerialDevice();

	void setDeviceInfo(EasyDevice::DeviceInfo deviceInfo);

	virtual const bool disconnect();

	virtual const QMap<QString, QString> information() const;
	virtual const bool available();
	virtual const bool compile(const QString& name);
	virtual const bool download(const QString& name, TinyArchive* archive);
	virtual const bool run(const QString& name);
	virtual const bool list();
	virtual const bool deleteProgram(const QString& name);
	virtual const bool interaction(const QString& command);
	virtual const bool authenticate(const QByteArray& hash);
	virtual const bool sendCustom(const QString& custom, const QByteArray& payload);
private slots:
	void readyRead();

private:
	void processPayload(const QByteArray& payload);
	bool connect();

	QString m_path;
	EasyDevice::DeviceInfo m_deviceInfo;
	SerialIODevice m_device;
	quint32 m_payload;
};

#endif
