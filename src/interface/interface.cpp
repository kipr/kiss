#include "interface.hpp"

using namespace kiss::target;

Interface::Interface(const QString &name)
	: m_name(name)
{
}

Interface::~Interface()
{
}

const QString &Interface::name() const
{
	return m_name;
}

void Interface::emitScanFinished()
{
  emit scanFinished(this);
}
	
