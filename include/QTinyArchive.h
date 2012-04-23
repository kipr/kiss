#ifndef _QTINYARCHIVE_H_
#define _QTINYARCHIVE_H_

#include <TinyArchive.h>

#include <QString>
#include <QStringList>
#include <QByteArray>

class QTinyNode
{
public:
	static QString name(const TinyNode* node);
	static QByteArray data(const TinyNode* node);
	static QString path(const TinyNode* node);
};

class QPathUtils
{
public:
	static QString appendComponent(const QString& path, const QString& c);
	static QString prependComponent(const QString& path, const QString& c);
	static QString lastComponent(const QString& path);
	static QString firstComponent(const QString& path);
	static QString removeLastComponent(const QString& path);
	static QString removeFirstComponent(const QString& path);
	static QStringList components(const QString& path);
	static QString path(const QStringList& components);
	static bool isNull(const QString& path);
};


class QTinyArchive : public TinyArchive
{
public:
	const QStringList files() const;
	
	const bool add(const QString& path, uint32_t id = 0);
	const bool add(const QString& path, const QByteArray& data, uint32_t id = 0);
	const bool update(const QString& path, const QByteArray& data, uint32_t id = 0);
	const bool put(const QString& path, const QByteArray& data, uint32_t id = 0);
	const bool remove(const QString& path);
	
	const bool exists(const QString& path) const;
	const TinyNode* lookup(const QString& path) const;
};

#endif
