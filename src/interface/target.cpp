#include "target.hpp"
#include "target_responder.hpp"

#include <QDebug>

using namespace kiss::target;

Target::Target(Interface *interface, const QString &name)
	: m_interface(interface)
{
	qRegisterMetaType<Response>("Response");
	connect(this, SIGNAL(response(Response)),
		SLOT(responseRedirect(Response)));
	qRegisterMetaType<kiss::target::Target::ReturnCode>("kiss::target::Target::ReturnCode");
}

Target::~Target()
{
}

Interface *Target::interface() const
{
	return m_interface;
}

QString Target::displayName() const
{
	return information().value(DISPLAY_NAME);
}

QString Target::commPort() const
{
	return information().value(COMM_PORT);
}

QString Target::type() const
{
	return information().value(TARGET_DEVICE_TYPE);
}

QString Target::serial() const
{
	return information().value(SERIAL);
}

QString Target::version() const {
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
