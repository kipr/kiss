#ifndef _INTERFACE_RESPONDER_HPP_
#define _INTERFACE_RESPONDER_HPP_

#include "target.hpp"

namespace kiss
{
	namespace target
	{
		class Interface;

		class InterfaceResponder
		{
		public:
			virtual void targetScanStarted(Interface *interface) = 0;
			virtual void targetFound(Interface *interface, TargetPtr target) = 0;
		};
	}
}

#endif
