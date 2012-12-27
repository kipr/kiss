#ifndef _KOVAN_PROTO_TARGET_HPP_
#define _KOVAN_PROTO_TARGET_HPP_

#include "target.hpp"

#include <kovanserial/transport_layer.hpp>
#include <kovanserial/kovan_serial.hpp>

class Transmitter;

class KovanProtoTarget : public QObject, public Target
{
Q_OBJECT
public:
	KovanProtoTarget(Transmitter *transmitter);
	~KovanProtoTarget();
	
	virtual const QMap<QString, QString> information() const; // Needs to be cached

	virtual const bool disconnect();

	virtual const bool available();
	virtual const bool compile(const QString& name);
	virtual const bool download(const QString& name, const KarPtr& archive);
	virtual const bool run(const QString& name);

	virtual const bool list();
	virtual const bool deleteProgram(const QString& name);
	virtual const bool interaction(const QString& command);

	virtual const bool authenticate(const QByteArray& hash);
	virtual const bool sendCustom(const QString& custom, const QByteArray& payload = QByteArray());
	
private:
	Transmitter *m_serial;
	TransportLayer m_transport;
	KovanSerial m_proto;
};

#endif
