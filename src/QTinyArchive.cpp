#include "QTinyArchive.h"

QStringList stdListToQStringList(const std::list<std::string>& raw)
{
	std::list<std::string>::const_iterator it = raw.begin();
	QStringList ret;
	for(; it != raw.end(); ++it) ret << QString::fromStdString(*it);
	return ret;
}

std::list<std::string> qStringListToStdList(const QStringList& raw)
{
	std::list<std::string> ret;
	foreach(const QString& r, raw) ret.push_back(r.toStdString());
	return ret;
}

QString QTinyNode::name(const TinyNode* node)
{
	return (node && node->name()) ? node->name() : QString();
}

QByteArray QTinyNode::data(const TinyNode* node)
{
	if(!node) return QByteArray();
	return QByteArray(reinterpret_cast<const char*>(node->data()), node->length());
}

QString QTinyNode::path(const TinyNode* node)
{
	return node ? QString::fromStdString(node->path()) : QString();
}

QString QPathUtils::appendComponent(const QString& path, const QString& c)
{
	return QString::fromStdString(PathUtils::appendComponent(path.toStdString(), c.toStdString()));
}

QString QPathUtils::prependComponent(const QString& path, const QString& c)
{
	return QString::fromStdString(PathUtils::prependComponent(path.toStdString(), c.toStdString()));
}

QString QPathUtils::lastComponent(const QString& path)
{
	return QString::fromStdString(PathUtils::lastComponent(path.toStdString()));
}

QString QPathUtils::firstComponent(const QString& path)
{
	return QString::fromStdString(PathUtils::firstComponent(path.toStdString()));
}

QString QPathUtils::removeLastComponent(const QString& path)
{
	return QString::fromStdString(PathUtils::removeLastComponent(path.toStdString()));
}

QString QPathUtils::removeFirstComponent(const QString& path)
{
	return QString::fromStdString(PathUtils::removeFirstComponent(path.toStdString()));
}

QStringList QPathUtils::components(const QString& path)
{
	return stdListToQStringList(PathUtils::components(path.toStdString()));
}

QString QPathUtils::path(const QStringList& components)
{
	return QString::fromStdString(PathUtils::path(qStringListToStdList(components)));
}

bool QPathUtils::isNull(const QString& path)
{
	return PathUtils::isNull(path.toStdString());
}

const QStringList QTinyArchive::files() const
{
	return files(this);
}

const QStringList QTinyArchive::files(const TinyArchive* archive)
{
	return stdListToQStringList(archive->TinyArchive::files());
}

const bool QTinyArchive::add(const QString& path, uint32_t id)
{
	return TinyArchive::add(path.toStdString(), 0, 0, id);
}

const bool QTinyArchive::add(const QString& path, const QByteArray& data, uint32_t id)
{
	return TinyArchive::add(path.toStdString(), reinterpret_cast<const unsigned char*>(data.data()), data.size(), id);
}

const bool QTinyArchive::update(const QString& path, const QByteArray& data, uint32_t id)
{
	return TinyArchive::update(path.toStdString(), reinterpret_cast<const unsigned char*>(data.data()), data.size(), id);
}

const bool QTinyArchive::put(const QString& path, const QByteArray& data, uint32_t id)
{
	return TinyArchive::put(path.toStdString(), reinterpret_cast<const unsigned char*>(data.data()), data.size(), id);
}

const bool QTinyArchive::remove(const QString& path)
{
	return TinyArchive::remove(path.toStdString());
}

const bool QTinyArchive::exists(const QString& path) const
{
	return TinyArchive::exists(path.toStdString());
}

const TinyNode* QTinyArchive::lookup(const QString& path) const
{
	return TinyArchive::lookup(path.toStdString());
}