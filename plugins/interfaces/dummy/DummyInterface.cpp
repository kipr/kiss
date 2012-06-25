#include "DummyInterface.h"

#include <QDebug>

class DummyDevice : public Device
{
public:
	DummyDevice(Interface* interface, bool source)
		: Device(interface, "Dummy"), m_source(source) {}
	
	virtual const QMap<QString, QString> information() const {
		QMap<QString, QString> ret;
		ret[DISPLAY_NAME] = QString("Dummy ") + (m_source ? "(Source)" : "(Binary)");
		ret[DEVICE_TYPE] = name();
		ret[DOWNLOAD_TYPE] = m_source ? DOWNLOAD_SOURCE : DOWNLOAD_BINARY;
		ret[SERIAL] = NOT_AVAILABLE;
		return ret;
	}
	
	virtual const bool available()
	{
		return true;
	}
	
	virtual const CompileResult compile(const QString& name)
	{
		qDebug() << "(Dummy Device) Compiling" << name;
		return CompileResult(true);
	}
	
	virtual const bool download(const QString& name, TinyArchive* archive)
	{
		qDebug() << "(Dummy Device) Downloading" << name;
		return true;
	}
	
	virtual const bool run(const QString& name)
	{
		qDebug() << "(Dummy Device) Running" << name;
		return true;
	}
private:
	bool m_source;
};

DummyInterface::DummyInterface() : Interface("Dummy Interface") {}

QList<DevicePtr> DummyInterface::devices()
{
	return QList<DevicePtr>()
		<< DevicePtr(new DummyDevice(this, true))
		<< DevicePtr(new DummyDevice(this, false));
}

const bool DummyInterface::local() { return false; }

Q_EXPORT_PLUGIN2(dummy_interface, DummyInterface);