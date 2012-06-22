#ifndef _PLATFORMHINTSMANAGER_H_
#define _PLATFORMHINTSMANAGER_H_

#include "Singleton.h"
#include <QString>
#include <QMap>

typedef QMap<QString, QString> FlagMap;

class PlatformHintsManager : public Singleton<PlatformHintsManager>
{
public:
	void setPlatformHints(const QString& platform, const FlagMap& m_flags);
	const FlagMap flagMap(const QString& platform) const;
	const QString flags(const QString& platform, const QString& compiler) const;
private:
	QMap<QString, FlagMap> m_hints;
};

#endif
