#include "kar.hpp"

#include <QFile>
#include <QIODevice>
#include <QDataStream>

using namespace Kiss;

#define MAX_FILENAME_SIZE 128

bool Kar::addFile(const QString& name, const QByteArray& data)
{
	if(m_data.contains(name)) return false;
	m_data.insert(name, data);
	return true;
}

void Kar::setFile(const QString& name, const QByteArray& data)
{
	m_data.remove(name);
	m_data.insert(name, data);
}

bool Kar::removeFile(const QString& name)
{
	if(!m_data.contains(name)) return false;
	m_data.remove(name);
	return true;
}

bool Kar::hasFile(const QString& name)
{
	return m_data.contains(name);
}

bool Kar::rename(const QString& name, const QString& newName)
{
	if(!m_data.contains(name) || m_data.contains(newName)) return false;
	
	m_data[newName] = m_data[name];
	m_data.remove(name);
}

QByteArray Kar::data(const QString& name) const
{
	return m_data.value(name, QByteArray());
}

QStringList Kar::files() const
{
	return QStringList(m_data.keys());
}

Kar *Kar::create()
{
	return new Kar();
}

Kar *Kar::load(const QString& path)
{
	QFile file(path);
	if(!file.open(QIODevice::ReadOnly)) return 0;
	QDataStream stream(&file);
	stream.setVersion(QDataStream::Qt_4_0);
	QMap<QString, QByteArray> compressedData;
	stream >> compressedData;
	QMap<QString, QByteArray> data;
	foreach(const QString& key, compressedData.keys()) {
		data.insert(key, qUncompress(compressedData[key]));
	}
	file.close();
	return new Kar(data);
}

bool Kar::save(const QString& path)
{
	QFile file(path);
	if(!file.open(QIODevice::WriteOnly)) return false;
	QDataStream stream(&file);
	stream.setVersion(QDataStream::Qt_4_0);
	QMap<QString, QByteArray> compressedData;
	foreach(const QString& key, m_data.keys()) {
		compressedData.insert(key, qCompress(m_data[key]));
	}
	stream << compressedData;
	file.close();
	return true;
}

Kar::Kar()
{
}

Kar::Kar(const QMap<QString, QByteArray>& data)
	: m_data(data)
{
}