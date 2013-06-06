#ifndef _TARGET_RESPONDER_HPP_
#define _TARGET_RESPONDER_HPP_

#include "target_response.hpp"

namespace Kiss
{
	namespace Target
	{
		class Target;
		
		class Responder
		{
		public:
			virtual ~Responder();
			
			virtual void response(Target *target, const Response &response) = 0; 
		};
	}
}

#endif
