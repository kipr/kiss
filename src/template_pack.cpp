#include "template_pack.hpp"

#include "resource_helper.hpp"

#include <QFileInfo>
#include <QDataStream>
#include <QBuffer>
#include <QImageWriter>
#include <QImageReader>
#include <QDebug>

using namespace Kiss;
using namespace Kiss::Template;

#define DESC_SUFFIX ":desc"
#define DISPLAY_NAME ":display_name"
#define LEXER_SUFFIX ":lexer"

Pack::~Pack()
{
}

void Pack::setName(const QString& name)
{
	if(Pack::name() == name) return;
	m_archive->setFile(DISPLAY_NAME, name.toLatin1());
	emit nameChanged(name);
}

QString Pack::name() const
{
	return QString(m_archive->data(DISPLAY_NAME));
}

const QString& Pack::loadedFrom() const
{
	return m_loadedFrom;
}

bool Pack::addFile(const QString& path, const Template::File& file)
{
	if(isSpecialFile(path)) return false;
	
	if(!m_archive->addFile(path, file.data())) return false;
	if(!setLexer(path, file.lexer())) return false;
	
	emit fileAdded(path, file);
	
	return true;
}

void Pack::setFile(const QString& path, const Template::File& file)
{
	if(isSpecialFile(path)) return;
	
	bool prev = m_archive->hasFile(path);
	m_archive->setFile(path, file.data());
	setLexer(path, file.lexer());
	if(!prev) emit fileAdded(path, file);
}

bool Pack::removeFile(const QString& path)
{
	m_archive->removeFile(path + DESC_SUFFIX);
	m_archive->removeFile(path + LEXER_SUFFIX);
	if(!m_archive->removeFile(path)) return false;
	emit fileRemoved(path);
	return true;
}

bool Pack::hasFile(const QString& path)
{
	if(path.isEmpty()) return true;
	return m_archive->hasFile(path);
}

bool Pack::rename(const QString& path, const QString& newPath)
{
	m_archive->rename(path + DESC_SUFFIX, newPath + DESC_SUFFIX);
	m_archive->rename(path + LEXER_SUFFIX, newPath + LEXER_SUFFIX);
	return m_archive->rename(path, newPath);
}

QStringList Pack::templates() const
{
	QStringList ret;
	foreach(const QString& file, m_archive->files()) {
		if(!isSpecialFile(file)) ret << file;
	}
	ret.sort();
	return ret;
}

QStringList Pack::allFiles() const
{
	QStringList ret = m_archive->files();
	ret.sort();
	return ret;
}

Template::File Pack::file(const QString& path) const
{
	return Template::File(m_archive->data(path), lexer(path));
}

bool Pack::setDescription(const QString& path, const QString& description)
{
	if(!hasFile(path)) return false;
	m_archive->setFile(path + DESC_SUFFIX, description.toLatin1());
	return true;
}

bool Pack::setDescription(const QString& description)
{
	return setDescription("", description);
}

bool Pack::hasDescription(const QString& path) const
{
	QByteArray descData(m_archive->data(path + DESC_SUFFIX));
	return !descData.isNull() && !descData.isEmpty();
}

bool Pack::hasDescription() const
{
	return hasDescription("");
}

QString Pack::description(const QString& path) const
{
	QByteArray descData(m_archive->data(path + DESC_SUFFIX));
	if(descData.isNull() || descData.isEmpty()) return QString();
	return QString(descData);
}

QString Pack::description() const
{
	return description("");
}

bool Pack::setLexer(const QString& path, const QString& lexer)
{
	if(!hasFile(path)) return false;
	m_archive->setFile(path + LEXER_SUFFIX, lexer.toAscii());
	return true;
}

QString Pack::lexer(const QString& path) const
{
	return QString(m_archive->data(path + LEXER_SUFFIX));
}

PackPtr Pack::create()
{
	Pack *pack = new Pack(Kar::create(), "");
	pack->setName(tr("My Awesome Pack"));
	return PackPtr(pack);
}

PackPtr Pack::load(const QString& path)
{
	KarPtr kar = Kar::load(path);
	if(kar.isNull()) return PackPtr();
	return PackPtr(new Pack(kar, path));
}

bool Pack::save(const QString& path) const
{
	return m_archive->save(path);
}

Pack::Pack(const KarPtr& archive, const QString& loadedFrom)
	: m_archive(archive),
	m_loadedFrom(loadedFrom)
{
}

bool Pack::isSpecialFile(const QString& path) const
{
	return path.endsWith(DESC_SUFFIX)
		|| path.endsWith(LEXER_SUFFIX)
		|| path.startsWith(":");
}