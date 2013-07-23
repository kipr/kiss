#include "linked_kar.hpp"

#define LINK_FILE (KAR_SPECIAL_PREFIX "links")
#define SEP "\n"

using namespace kiss;
using namespace kiss::project;

bool LinkedKar::addLink(const kiss::KarPtr &archive, const QString &path)
{
	if(archive.isNull()) return false;
	QByteArray data = archive->data(LINK_FILE);
	data += path + SEP;
	archive->setFile(LINK_FILE, data);
	return true;
}

bool LinkedKar::removeLink(const kiss::KarPtr &archive, const QString &path)
{
	if(archive.isNull()) return false;
	
	QStringList lines = links(archive);
	QStringList newLines;
	
	
	bool hit = false;
	foreach(const QString &line, lines) {
		if(line == path) {
			hit = true;
			continue;
		}
		newLines << line;
	}
	
	archive->setFile(LINK_FILE, newLines.join(SEP).toUtf8());
	return hit;
}

QStringList LinkedKar::links(const kiss::KarPtr &archive)
{
	if(archive.isNull()) return QStringList();
	return QString(archive->data(LINK_FILE)).split(SEP);
}

LinkedKar::LinkedKar()
{
}