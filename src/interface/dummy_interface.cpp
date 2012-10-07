#include "dummy_interface.hpp"

#include "interface_manager.hpp"
#include "target.hpp"

using namespace Kiss;
using namespace Kiss::Target;

class DummyTarget : public Kiss::Target::Target
{
public:
	DummyTarget(Interface *parent)
		: Target(parent, "Dummy Target")
	{
		
	}
	
	virtual const QMap<QString, QString> information() const
	{
		QMap<QString, QString> ret;
		ret[DISPLAY_NAME] = "No Target";
		ret[DEVICE_TYPE] = "Dummy";
		return ret;
	}

	virtual const bool disconnect()
	{
		return false;
	}

	virtual const bool available()
	{
		return false;
	}
	
	virtual const bool compile(const QString& name)
	{
		return false;
	}
	
	virtual const bool download(const QString& name)
	{
		return false;
	}
	
	virtual const bool run(const QString& name)
	{
		return false;
	}

	virtual const bool list()
	{
		return false;
	}
	
	virtual const bool deleteProgram(const QString& name)
	{
		return false;
	}
	
	virtual const bool interaction(const QString& command)
	{
		return false;
	}

	virtual const bool authenticate(const QByteArray& hash)
	{
		return false;
	}
	
	virtual const bool sendCustom(const QString& custom, const QByteArray& payload = QByteArray())
	{
		return false;
	}
};

__attribute__((constructor))
static void __registerDummy()
{
	InterfaceManager::ref().addInterface(new DummyInterface());
}

DummyInterface::DummyInterface()
	: Interface("Dummy")
{
	
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