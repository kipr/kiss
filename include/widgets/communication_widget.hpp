#ifndef _COMMUNICATION_WIDGET_HPP_
#define _COMMUNICATION_WIDGET_HPP_

#include <QWidget>

namespace Ui
{
	class CommunicationWidget;
}

namespace Kiss
{
	namespace Widget
	{
		class CommunicationWidget : public QWidget
		{
		public:
			CommunicationWidget(QWidget *parent = 0);
			~CommunicationWidget();
			
		private:
			Ui::CommunicationWidget *ui;
		};
	}
}

#endif
