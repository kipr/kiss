#include "interface.hpp"

using namespace Kiss::Target;

Interface::Interface(const QString& name)
	: m_name(name)
{
}

const QString& Interface::name() const
{
	return m_name;
}
	
