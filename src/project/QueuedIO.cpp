#include "QueuedIO.h"

#include <QFileInfo>
#include <QDir>
#include <QFile>
#include <QDebug>

QueuedJob::~QueuedJob()
{
	
}

WriteFileQueuedJob::WriteFileQueuedJob(const QString& path, const QByteArray& data)
	: m_path(path), m_data(data)
{
}

bool WriteFileQueuedJob::execute()
{
	QFile file(m_path);
	if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) return false;
	
	file.write(m_data);
	
	file.close();
	
	return true;
}

QString WriteFileQueuedJob::description() const
{
	return m_path + " (write file)";
}

MakeDirectoryQueuedJob::MakeDirectoryQueuedJob(const QString& path) : m_path(path)
{
}

bool MakeDirectoryQueuedJob::execute()
{
	return QDir(m_path).mkdir("");
}

QString MakeDirectoryQueuedJob::description() const
{
	return m_path + " (create dir)";
}

RemoveFileQueuedJob::RemoveFileQueuedJob(const QString& path) : m_path(path)
{
}

bool RemoveFileQueuedJob::execute()
{
	QFileInfo fi(m_path);
	if(fi.isDir()) return fi.dir().rmdir("");
	return QFile::remove(fi.absoluteFilePath());
}

QString RemoveFileQueuedJob::description() const
{
	return m_path + " (rm)";
}

QueuedIO::~QueuedIO()
{
	QueuedJob* job = 0;
	while((job = dequeue())) delete job;
}

bool QueuedIO::execute()
{
	bool ret = true;
	while(m_jobs.size()) {
		QueuedJob* job = dequeue();
		bool success = job->execute();
		if(!success) qWarning() << job->description() << "FAILED!";
		ret &= success;
		delete job;
	}
	return ret;
}

void QueuedIO::enqueue(QueuedJob* job)
{
	m_jobs.append(job);
}

QueuedJob* QueuedIO::dequeue()
{
	return m_jobs.takeFirst();
}