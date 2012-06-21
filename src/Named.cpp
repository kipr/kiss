#include "Named.h"

Named::Named(const QString& name) : m_name(name)
{
	
}

const QString& Named::name() const
{
	return m_name;
}