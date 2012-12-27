#include "kovan_proto_target.hpp"

#include <QQueue>
#include <QMutex>
#include <QRunnable>
#include <QThread>
#include <QThreadPool>

struct Work
{
	enum Type {
		Downlaod,
		Compile,
		Run
	};
	
	Type type;
	QString name;
	KarPtr archive;
};

class KovanSerialWorker : public QRunnable
{
Q_OBJECT
public:
	KovanSerialWorker(KovanProto *proto)
		: m_proto(proto),
		m_stop(false)
	{
	}
	
	void work(const Work &w)
	{
		m_mutex.lock();
		m_workQueue.enqueue(w);
		m_mutex.unlock();
	}
	
	void stop()
	{
		m_stop = true;
	}
	
	void run()
	{
		while(!m_stop) {
			m_mutex.lock();
			Work w = m_workQueue.dequeue();
			m_mutex.unlock();
			
			QThread::yieldCurrentThread();
		}
	}
	
private:
	bool download(const Work &w)
	{
		m_proto->sendFile();
	}
	
	KovanProto *m_proto;
	QMutex m_mutex;
	bool m_stop;
	QQueue<Work> m_workQueue;
};

KovanProtoTarget::KovanProtoTarget(Transmitter *transmitter)
	: m_transmitter(transmitter),
	m_transport(m_transmitter),
	m_proto(&m_transport)
{
	
}

KovanProtoTarget::~KovanProtoTarget()
{
	delete m_transmitter;
}

const QMap<QString, QString> KovanProtoTarget::information() const
{
	return QMap<QString, QString>();
}

const bool KovanProtoTarget::disconnect()
{
	// We will be auto-disconnected by libkovanserial
}

const bool KovanProtoTarget::available()
{
	return true;
}

const bool KovanProtoTarget::compile(const QString& name)
{
	
}

const bool KovanProtoTarget::download(const QString& name, const KarPtr& archive)
{
	return m_proto.sendFile(name);
}

const bool KovanProtoTarget::run(const QString& name)
{
	
}

const bool KovanProtoTarget::list()
{
	
}

const bool KovanProtoTarget::deleteProgram(const QString& name)
{
	
}

const bool KovanProtoTarget::interaction(const QString& command)
{
	
}

const bool KovanProtoTarget::authenticate(const QByteArray& hash)
{
	
}

const bool KovanProtoTarget::sendCustom(const QString& custom, const QByteArray& payload)
{
	
}