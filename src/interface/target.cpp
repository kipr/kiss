#include "target.hpp"
#include "target_responder.hpp"

#include <QDebug>

using namespace Kiss::Target;

Target::Target(Interface* interface, const QString& name)
	: m_interface(interface)
{
	
}

Target::~Target()
{
	
}

Interface *Target::interface() const
{
	return m_interface;
}

const QString Target::displayName() const
{
	return information().value(DISPLAY_NAME);
}

const QString Target::commPort() const
{
	return information().value(COMM_PORT);
}

const QString Target::type() const
{
	return information().value(DEVICE_TYPE);
}

const QString Target::serial() const
{
	return information().value(SERIAL);
}

const QString Target::version() const {
	return information().value(VERSION);
}

void Target::addResponder(Responder *responder)
{
	m_responders.append(responder);
}

void Target::addResponders(const ResponderPtrList& responders)
{
	m_responders.append(responders);
}

void Target::removeResponder(Responder *responder)
{
	m_responders.removeAll(responder);
}

void Target::clearResponders()
{
	m_responders.clear();
}

const ResponderPtrList& Target::responders() const
{
	return m_responders;
}

