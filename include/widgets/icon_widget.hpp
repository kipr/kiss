#ifndef _ICON_WIDGET_HPP_
#define _ICON_WIDGET_HPP_

#include <QLabel>
#include <QIcon>

namespace kiss
{
	namespace widget
	{
		class Icon : public QLabel
		{
		Q_OBJECT
		public:
			Icon(QWidget *parent = 0);
			
			void setIcon(const QIcon &icon);
			QIcon icon() const;
			
		signals:
			void clicked();
		
		protected:
			void mousePressEvent(QMouseEvent *event);
		};
	}
}

#endif
