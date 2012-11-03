#ifndef _LOG_WINDOW_HPP_
#define _LOG_WINDOW_HPP_

#include <QWidget>

#include "ui_LogWindow.h"
#include "singleton.hpp"

#include "log_stream_buf.hpp"

namespace Kiss
{
	namespace Widget
	{
		class LogWindow : public QWidget, private Ui::LogWindow, public Singleton<LogWindow>
		{
		public:
			LogWindow();
			void append(const QString& str);
		};
	}
}

#endif
