#include "dummy_interface.hpp"

#include "interface_manager.hpp"
#include "target.hpp"

using namespace Kiss;

class DummyTarget : public Kiss::Target::Target
{
public:
	DummyTarget(Kiss::Target::Interface *parent)
		: Target(parent, "Dummy Target")
	{
		
	}
	
	virtual QMap<QString, QString> information() const
	{
		QMap<QString, QString> ret;
		ret[DISPLAY_NAME] = "No Target";
		ret[DEVICE_TYPE] = "Dummy";
		return ret;
	}

	virtual bool disconnect()
	{
		return false;
	}

	virtual bool available()
	{
		return false;
	}
	
	virtual Target::ReturnCode compile(quint64 id, const QString &name)
	{
		return Target::NotImplemented;
	}
	
	virtual Target::ReturnCode download(quint64 id, const QString &name, const KarPtr &archive)
	{
		return Target::NotImplemented;
	}
	
	virtual Target::ReturnCode run(quint64 id, const QString &name)
	{
		return Target::NotImplemented;
	}

	virtual Target::ReturnCode list(quint64 id)
	{
		return Target::NotImplemented;
	}
	
	virtual Target::ReturnCode deleteProgram(quint64 id, const QString &name)
	{
		return Target::NotImplemented;
	}
	
	virtual Target::ReturnCode interaction(quint64 id, const QString &command)
	{
		return Target::NotImplemented;
	}
	
	virtual Target::ReturnCode sendCustom(quint64 id, const QString &custom, const QByteArray &payload = QByteArray())
	{
		return Target::NotImplemented;
	}
	
	virtual bool setPassword(const QString &password)
	{
		return true;
	}
	
	virtual void clearPassword()
	{

	}
};

using namespace Kiss::Target;

__attribute__((constructor))
static void __registerDummy()
{
	InterfaceManager::ref().addInterface(new DummyInterface());
}

DummyInterface::DummyInterface()
	: Interface("Dummy")
{
	
}

TargetPtr DummyInterface::createTarget(const QString &address)
{
	return TargetPtr();
}

const bool DummyInterface::scan(Kiss::Target::InterfaceResponder *responder)
{
	responder->targetScanStarted(this);
	responder->targetFound(this, TargetPtr(new DummyTarget(this)));
	return true;
}

void DummyInterface::invalidateResponder()
{
	
}