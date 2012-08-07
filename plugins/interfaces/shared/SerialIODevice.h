#ifndef _SERIALIODEVICE_H_
#define _SERIALIODEVICE_H_

#include <QIODevice>

class SerialIODevice : public QIODevice
{
public:
	SerialIODevice(const QString& path);
	~SerialIODevice();
	
	virtual bool open(QIODevice::OpenMode mode);
	virtual void close();
	
	void setBaudRate(const quint32& baudRate);
	quint32 baudRate() const;
	
#ifndef WIN32
	bool select(const quint32& msecs);
#endif
	
protected:
	virtual qint64 readData(char *data, qint64 maxSize);
	virtual qint64 writeData(const char *data, qint64 maxSize);
	
private:
	void configurePort();
	
	QString m_path;
	
	int m_fd;
	quint32 m_baudRate;
};

#endif
