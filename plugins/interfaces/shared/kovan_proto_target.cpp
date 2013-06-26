#include "kovan_proto_target.hpp"

#include "keys.hpp"
#include <kovanserial/transmitter.hpp>
#include <pcompiler/output.hpp>

#include <QQueue>
#include <QDebug>
#include <sstream>

using namespace Kiss;
using namespace Kiss::Target;

KovanProtoTarget::KovanProtoTarget(Transmitter *transmitter, Interface *interface)
	: Target(interface, "kovan_proto_target"),
	m_transmitter(transmitter),
	m_transport(m_transmitter),
	m_proto(&m_transport)
{
}

KovanProtoTarget::~KovanProtoTarget()
{
	delete m_transmitter;
}

void KovanProtoTarget::fillDisplayName(const QString &displayName)
{
	m_information[DISPLAY_NAME] = displayName;
}

void KovanProtoTarget::fillCommPort(const QString &commPort)
{
	m_information[COMM_PORT] = commPort;
}

void KovanProtoTarget::fillInformation(const Advert &advert)
{
	m_information[DISPLAY_NAME] = advert.name;
	m_information[SERIAL] = advert.serial;
	m_information[VERSION] = advert.version;
	m_information[DEVICE_TYPE] = advert.device;
}

QMap<QString, QString> KovanProtoTarget::information() const
{
	return m_information;
}

bool KovanProtoTarget::disconnect()
{
	// We will be auto-disconnected by libkovanserial
	return true;
}

bool KovanProtoTarget::available()
{
	return true;
}

#define KEY_COMPILE "compile"

Kiss::Target::Target::ReturnCode KovanProtoTarget::compile(quint64 id, const QString& name)
{
	emit response(Response(id, "began_" KEY_COMPILE));
	qDebug() << "Compiling...";
	bool success = m_transmitter->makeAvailable();
	if(!success) perror("makeAvailable");
	
	emit response(Response(id, "connection", success));
	
	if(!success) return Target::Error;
	
	{
		Target::ReturnCode ret = Target::Error;
		if((ret = verifyProtocolVersion()) != Target::Success) {
			m_proto.hangup();
			m_transmitter->endSession();
			return ret;
		}
	}
	
	{
		Target::ReturnCode ret = Target::Error;
		if((ret = authenticate(id)) != Target::Success) {
			m_proto.hangup();
			m_transmitter->endSession();
			return ret;
		}
	}

	
	qDebug() << "Compiling" << name;
	if(!m_proto.sendFileAction(COMMAND_ACTION_COMPILE, name.toStdString())) {
		emit response(Response(id, KEY_COMPILE, false));
		qWarning() << "Send file action failed.";
		m_proto.hangup();
		m_transmitter->endSession();
		return Target::Error;
	}
	
	bool finished = false;
	// TODO: Show progress
	double progress = 0.0;
	qDebug() << "Waiting for progress reports...";
	do {
		// TODO: Adjust this timeout
		if(!m_proto.recvFileActionProgress(finished, progress, 30000)) {
			emit response(Response(id, KEY_COMPILE, false));
			qWarning() << "recv file action progress failed.";
			m_proto.hangup();
			m_transmitter->endSession();
			return Target::Error;
		}
	} while(!finished);
	
	qDebug() << "Waiting for results...";
	
	Packet p;
	if(m_transport.recv(p, 15000) != TransportLayer::Success || p.type != Command::FileHeader) {
		qWarning() << "Failed to recv results of compile" << p.type;
		emit response(Response(id, KEY_COMPILE, true));
		m_proto.hangup();
		m_transmitter->endSession();
		return Target::Error;
	}
	
	Command::FileHeaderData header;
	p.as(header);
	
	if(!m_proto.confirmFile(true)) {
		qWarning() << "Failed to confirm incoming compile results";
		m_proto.hangup();
		m_transmitter->endSession();
		return Target::Error;
	};
	
	std::ostringstream sstream;
	if(!m_proto.recvFile(header.size, &sstream, 5000)) {
		qWarning() << "Recv compile results failed";
		m_proto.hangup();
		m_transmitter->endSession();
		return Target::Error;
	}
	
	m_proto.hangup();
	
	QByteArray compileResults(sstream.str().c_str(), sstream.str().size());
	QDataStream stream(&compileResults, QIODevice::ReadOnly);
	
	Compiler::OutputList results;
	stream >> results;

	foreach(Compiler::Output o, results) {
		qDebug() << o.error();
	}
	
	emit response(Response(id, KEY_COMPILE, QVariant::fromValue(results)));
	
	m_transmitter->endSession();
	
	return Target::Success;
}

#define KEY_DOWNLOAD "download"

Kiss::Target::Target::ReturnCode KovanProtoTarget::download(quint64 id, const QString& name, const KarPtr& archive)
{
	emit response(Response(id, "began_" KEY_DOWNLOAD));
	qDebug() << "Downloading" << name << "...";
	bool success = m_transmitter->makeAvailable();
	if(!success) perror("makeAvailable");
	emit response(Response(id, "connection", success));
	if(!success) return Target::Error;
	
	{
		Target::ReturnCode ret = Target::Error;
		if((ret = verifyProtocolVersion()) != Target::Success) {
			m_proto.hangup();
			m_transmitter->endSession();
			return ret;
		}
	}
	
	{
		Target::ReturnCode ret = Target::Error;
		if((ret = authenticate(id)) != Target::Success) {
			m_proto.hangup();
			m_transmitter->endSession();
			return ret;
		}
	}


	
	QByteArray data;
	QDataStream qStream(&data, QIODevice::ReadWrite);
	qStream << (*archive);
	std::istringstream istream;
	istream.rdbuf()->pubsetbuf(data.data(), data.size());
	if(!m_proto.sendFile(name.toStdString(), "kar", &istream)) {
		emit response(Response(id, KEY_DOWNLOAD, false));
		m_proto.hangup();
		m_transmitter->endSession();
		return Target::Error;
	}
	
	emit response(Response(id, KEY_DOWNLOAD, true));
	
	m_proto.hangup();
	m_transmitter->endSession();
	return Target::Success;
}

#define KEY_RUN "run"

Kiss::Target::Target::ReturnCode KovanProtoTarget::run(quint64 id, const QString& name)
{
	emit response(Response(id, "began_" KEY_RUN));
	qDebug() << "Running...";
	bool success = m_transmitter->makeAvailable();
	if(!success) perror("makeAvailable");
	emit response(Response(id, "connection", success));
	if(!success) return Target::Error;
	
	{
		Target::ReturnCode ret = Target::Error;
		if((ret = verifyProtocolVersion()) != Target::Success) {
			m_proto.hangup();
			m_transmitter->endSession();
			return ret;
		}
	}
	
	{
		Target::ReturnCode ret = Target::Error;
		if((ret = authenticate(id)) != Target::Success) {
			m_proto.hangup();
			m_transmitter->endSession();
			return ret;
		}
	}
	
	if(!m_proto.sendFileAction(COMMAND_ACTION_RUN, name.toStdString())) {
		emit response(Response(id, KEY_RUN, false));
		m_proto.hangup();
		return Target::Error;
	}
	
	bool finished = false;
	// TODO: Show progress
	double progress = 0.0;
	do {
		if(!m_proto.recvFileActionProgress(finished, progress, 5000)) {
			emit response(Response(id, KEY_RUN, false));
			m_proto.hangup();
			m_transmitter->endSession();
			return Target::Error;
		}
	} while(!finished);
	m_proto.hangup();
	m_transmitter->endSession();
	
	emit response(Response(id, KEY_RUN, true));
	
	return Target::Success;
}

Kiss::Target::Target::ReturnCode KovanProtoTarget::list(quint64 id)
{
	return Target::NotImplemented;
}

Kiss::Target::Target::ReturnCode KovanProtoTarget::deleteProgram(quint64 id, const QString& name)
{
	return Target::NotImplemented;
}

Kiss::Target::Target::ReturnCode KovanProtoTarget::interaction(quint64 id, const QString& command)
{
	return Target::NotImplemented;
}

Kiss::Target::Target::ReturnCode KovanProtoTarget::sendCustom(quint64 id, const QString& custom, const QByteArray& payload)
{
	return Target::NotImplemented;
}

bool KovanProtoTarget::setPassword(const QString &password)
{
	return m_proto.setPassword(password.toStdString());
}

void KovanProtoTarget::clearPassword()
{
	m_proto.setNoPassword();
}

Kiss::Target::Target::ReturnCode KovanProtoTarget::verifyProtocolVersion()
{
	// All firmwares will reply to
	// knock knock. This helps us
	// distinguish between the next case,
	// which is whether a timeout means no
	// communication or the message isn't implemented
	if(!m_proto.knockKnock(3000)) return Target::Error;
	
	std::string version;
	if(!m_proto.protocolVersion(version)) {
		// This message wasn't introduced until recently, so
		// older firmwares will ignore it.
		return Target::OldDeviceSoftware;
	}
	const int cmp = version.compare(KOVAN_SERIAL_PROTOCOL_VERSION);
	if(cmp == 0) return Target::Success;
	return cmp > 0 ? Target::OldDeviceSoftware : Target::OldHostSoftware;
}

Kiss::Target::Target::ReturnCode KovanProtoTarget::authenticate(const quint64 id)
{
	bool authNecessary = false;
	if(!m_proto.authenticationInfo(authNecessary)) {
		std::cerr << "Failed to fetch authorization info." << std::endl;
		emit response(Response(id, AUTHENTICATION_KEY, false));
		return Target::Error;
	}
	
	if(!authNecessary) {
		std::cout << "Authorized (No authorization necessary)" << std::endl;
		emit response(Response(id, AUTHENTICATION_KEY, true));
		return Target::Success;
	}
	
	bool authed = false;
	uint8_t sessionKey[KOVAN_SERIAL_SESSION_KEY_SIZE];
	if(!m_proto.requestAuthentication(authed, sessionKey) || !authed) {
		std::cout << "Authorization failed. Try again." << std::endl;
	}
	
	emit response(Response(id, AUTHENTICATION_KEY, authed));
	return authed ? Target::Success : Target::AuthenticationFailed;
}