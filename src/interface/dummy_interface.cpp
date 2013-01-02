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
	
	virtual const bool compile(quint64 id, const QString& name)
	{
		return false;
	}
	
	virtual const bool download(quint64 id, const QString& name, const KarPtr& archive)
	{
		return false;
	}
	
	virtual const bool run(quint64 id, const QString& name)
	{
		return false;
	}

	virtual const bool list(quint64 id)
	{
		return false;
	}
	
	virtual const bool deleteProgram(quint64 id, const QString& name)
	{
		return false;
	}
	
	virtual const bool interaction(quint64 id, const QString& command)
	{
		return false;
	}

	virtual const bool authenticate(quint64 id, const QByteArray& hash)
	{
		return false;
	}
	
	virtual const bool sendCustom(quint64 id, const QString& custom, const QByteArray& payload = QByteArray())
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