#ifndef _MAIN_RESPONDER_HPP_
#define _MAIN_RESPONDER_HPP_

#include "target_responder.hpp"

namespace kiss
{
	namespace widget
	{
		class MainWindow;
	}
	
	namespace target
	{
		class MainResponder : public Responder
		{
		public:
			MainResponder(widget::MainWindow *mainWindow);
			virtual ~MainResponder();
			
			virtual void response(Target *target, const Response &response);
			
		private:
			widget::MainWindow *m_mainWindow;
		};
	}
}

#endif
