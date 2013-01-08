#include "target.hpp"
#include "target_responder.hpp"

#include <QDebug>

using namespace Kiss::Target;

Target::Target(Interface* interface, const QString& name)
	: m_interface(interface)
{
	qRegisterMetaType<Response>("Response");
	connect(this, SIGNAL(response(Response)),
		SLOT(responseRedirect(Response)));
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

void Target::Target::setResponder(Responder *responder)
{
	m_responder = responder;
}

Responder *Target::responder() const
{
	return m_responder;
}

void Target::responseRedirect(const Response &response)
{
	if(!m_responder) return;
	m_responder->response(this, response);
}
