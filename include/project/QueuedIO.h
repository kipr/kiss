#ifndef _QUEUEDIO_H_
#define _QUEUEDIO_H_

#include <QList>
#include <QString>

struct QueuedJob
{
	virtual ~QueuedJob();
	virtual bool execute() = 0;
	
	virtual QString description() const = 0;
};

struct WriteFileQueuedJob : QueuedJob
{
	WriteFileQueuedJob(const QString& path, const QByteArray& data);
	
	virtual bool execute();
	virtual QString description() const;
private:
	QString m_path;
	QByteArray m_data;
};

struct MakeDirectoryQueuedJob : QueuedJob
{
	MakeDirectoryQueuedJob(const QString& path);
	
	virtual bool execute();
	virtual QString description() const;
private:
	QString m_path;
};

struct RemoveFileQueuedJob : QueuedJob
{
	RemoveFileQueuedJob(const QString& path);
	
	virtual bool execute();
	virtual QString description() const;
private:
	QString m_path;
};

class QueuedIO 
{
public:
	~QueuedIO();
	
	bool execute();
	
	void enqueue(QueuedJob* job);
	QueuedJob* dequeue();
	
private:
	QList<QueuedJob*> m_jobs;
};



#endif
