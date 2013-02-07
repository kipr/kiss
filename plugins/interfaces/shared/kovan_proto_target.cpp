#include "kovan_proto_target.hpp"

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

const QMap<QString, QString> KovanProtoTarget::information() const
{
	return m_information;
}

const bool KovanProtoTarget::disconnect()
{
	// We will be auto-disconnected by libkovanserial
	return true;
}

const bool KovanProtoTarget::available()
{
	return true;
}

#define KEY_COMPILE "compile"

const bool KovanProtoTarget::compile(quint64 id, const QString& name)
{
	emit response(Response(id, "began_" KEY_COMPILE));
	qDebug() << "Compiling...";
	bool success = m_transmitter->makeAvailable();
	if(!success) perror("makeAvailable");
	emit response(Response(id, "connection", success));
	if(!success) return false;
	
	qDebug() << "Compiling" << name;
	if(!m_proto.sendFileAction(COMMAND_ACTION_COMPILE, name.toStdString())) {
		emit response(Response(id, KEY_COMPILE, false));
		qWarning() << "Send file action failed.";
		m_proto.hangup();
		m_transmitter->endSession();
		return false;
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
			m_transmitter->endSession();
			return false;
		}
	} while(!finished);
	
	qDebug() << "Waiting for results...";
	
	Packet p;
	if(m_transport.recv(p, 15000) != Transmitter::Success || p.type != Command::FileHeader) {
		qWarning() << "Failed to recv results of compile" << p.type;
		emit response(Response(id, KEY_COMPILE, true));
		m_transmitter->endSession();
		return false;
	}
	
	Command::FileHeaderData header;
	p.as(header);
	
	if(!m_proto.confirmFile(true)) {
		qWarning() << "Failed to confirm incoming compile results";
		m_proto.hangup();
		m_transmitter->endSession();
		return false;
	};
	
	std::ostringstream sstream;
	if(!m_proto.recvFile(header.size, &sstream, 5000)) {
		qWarning() << "Recv compile results failed";
		m_proto.hangup();
		m_transmitter->endSession();
		return false;
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
	
	return true;
}

#define KEY_DOWNLOAD "download"

const bool KovanProtoTarget::download(quint64 id, const QString& name, const KarPtr& archive)
{
	emit response(Response(id, "began_" KEY_DOWNLOAD));
	qDebug() << "Downloading" << name << "...";
	bool success = m_transmitter->makeAvailable();
	if(!success) perror("makeAvailable");
	emit response(Response(id, "connection", success));
	if(!success) return false;
	QByteArray data;
	QDataStream qStream(&data, QIODevice::ReadWrite);
	qStream << (*archive);
	std::istringstream istream;
	istream.rdbuf()->pubsetbuf(data.data(), data.size());
	if(!m_proto.sendFile(name.toStdString(), "kar", &istream)) {
		emit response(Response(id, KEY_DOWNLOAD, false));
		m_proto.hangup();
		m_transmitter->endSession();
		return false;
	}
	
	emit response(Response(id, KEY_DOWNLOAD, true));
	
	m_proto.hangup();
	m_transmitter->endSession();
	return true;
}

#define KEY_RUN "run"

const bool KovanProtoTarget::run(quint64 id, const QString& name)
{
	emit response(Response(id, "began_" KEY_RUN));
	qDebug() << "Running...";
	bool success = m_transmitter->makeAvailable();
	if(!success) perror("makeAvailable");
	emit response(Response(id, "connection", success));
	if(!success) return false;
	
	if(!m_proto.sendFileAction(COMMAND_ACTION_RUN, name.toStdString())) {
		emit response(Response(id, KEY_RUN, false));
		m_proto.hangup();
		return false;
	}
	
	bool finished = false;
	// TODO: Show progress
	double progress = 0.0;
	do {
		if(!m_proto.recvFileActionProgress(finished, progress, 5000)) {
			emit response(Response(id, KEY_RUN, false));
			return false;
		}
	} while(!finished);
	m_proto.hangup();
	m_transmitter->endSession();
	
	emit response(Response(id, KEY_RUN, true));
	
	return true;
}

const bool KovanProtoTarget::list(quint64 id)
{
	return false;
}

const bool KovanProtoTarget::deleteProgram(quint64 id, const QString& name)
{
	return false;
}

const bool KovanProtoTarget::interaction(quint64 id, const QString& command)
{
	return false;
}

const bool KovanProtoTarget::authenticate(quint64 id, const QByteArray& hash)
{
	return false;
}

const bool KovanProtoTarget::sendCustom(quint64 id, const QString& custom, const QByteArray& payload)
{
	return false;
}