#include "ThisMachineTarget.h"

#include "ArchiveWriter.h"
#include "Temporary.h"

#define SUBDIR "this_machine"

class ThisMachineDevice : public Device
{
public:
	ThisMachineDevice() : Device("This Machine")
	{
		m_info[DISPLAY_NAME] = name();
		m_info[SERIAL] = NOT_AVAILABLE;
		m_info[DOWNLOAD_TYPE] = DOWNLOAD_BIN;
	}
	
	virtual const QMap<QString, QString> information()
	{
		return m_info;
	}
	
	virtual const bool available()
	{
		return true;
	}
	
	virtual const bool download(const QString& name, TinyArchive* archive)
	{
		QDir temp = Temporary::subdir(SUBDIR);
		ArchiveWriter writer(archive, temp);
		writer.write();
		qDebug() << "Downloaded" << name << "to this machine";
	}
	
	virtual const bool run(const QString& name)
	{
		qDebug() << "Running" << name;
	}
private:
	QMap<QString, QString> m_info;
};

class ThisMachineInterface : public DeviceInterface
{
public:
	DeviceInterface(const QString& name) : Named("This Machine Interface") {}
	
	virtual QList<DevicePtr> devices()
	{
		return QList<DevicePtr>() << DevicePtr(new ThisMachineDevice());
	}
private:
	
};

ThisMachineTarget::ThisMachineTarget()
	: Target("This Machine", QList<DeviceInterface*>() << new ThisMachineInterface()) {}