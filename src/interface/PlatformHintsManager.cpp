#include "PlatformHintsManager.h"

void PlatformHintsManager::setPlatformHints(const QString& platform, const FlagMap& m_flags)
{
	m_hints[platform] = m_flags;
}

const FlagMap PlatformHintsManager::flagMap(const QString& platform) const
{
	return m_hints.value(platform);
}

const QString PlatformHintsManager::flags(const QString& platform, const QString& compiler) const
{
	return flagMap(platform).value(compiler);
}