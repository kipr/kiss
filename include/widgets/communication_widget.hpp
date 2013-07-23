#ifndef _COMMUNICATION_WIDGET_HPP_
#define _COMMUNICATION_WIDGET_HPP_

#include <QWidget>

namespace Ui
{
	class CommunicationWidget;
}

namespace kiss
{
	namespace widget
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
