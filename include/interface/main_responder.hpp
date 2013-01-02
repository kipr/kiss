#ifndef _MAIN_RESPONDER_HPP_
#define _MAIN_RESPONDER_HPP_

#include "target_responder.hpp"

namespace Kiss
{
	namespace Widget
	{
		class MainWindow;
	}
	
	namespace Target
	{
		class MainResponder : public Responder
		{
		public:
			MainResponder(Widget::MainWindow *mainWindow);
			virtual ~MainResponder();
			
			virtual void response(Target *target, const Response &response);
			
		private:
			Widget::MainWindow *m_mainWindow;
		};
	}
}

#endif
