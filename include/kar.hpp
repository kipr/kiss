#ifndef _KAR_HPP_
#define _KAR_HPP_

#include <QString>
#include <QMap>
#include <QStringList>
#include <QByteArray>

namespace Kiss
{
	class Kar
	{
	public:	
		bool addFile(const QString& name, const QByteArray& data);
		void setFile(const QString& name, const QByteArray& data);
		bool removeFile(const QString& name);
		bool hasFile(const QString& name);
		bool rename(const QString& name, const QString& newName);
		
		QByteArray data(const QString& name) const;
		QStringList files() const;
		
		static Kar *create();
		static Kar *load(const QString& path);
		bool save(const QString& path);
		
	private:
		Kar();
		Kar(const QMap<QString, QByteArray>& data);
		
		QMap<QString, QByteArray> m_data;
	};
}

#endif
