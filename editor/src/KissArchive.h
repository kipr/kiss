#ifndef __KISSARCHIVE_H__
#define __KISSARCHIVE_H__

#include <QStringList>
#include <QIODevice>

class KissArchive 
{
public:
	static bool create(const QStringList& files, QIODevice* out);
	static bool extract(QIODevice* in);
private:
};

#endif