#include "Named.h"

Named(const QString& name) : m_name(name)
{
	
}

const QString& name() const
{
	return m_name;
}