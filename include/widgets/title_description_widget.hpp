#ifndef _TITLE_DESCRIPTION_WIDGET_HPP_
#define _TITLE_DESCRIPTION_WIDGET_HPP_

#include "ui_TitleDescriptionWidget.h"
#include <QString>

namespace Kiss
{
	namespace Widget
	{
		class TitleDescription : public QWidget, public Ui::TitleDescriptionWidget
		{
		Q_OBJECT
		public:
			TitleDescription(QWidget* parent);
			TitleDescription(const QString& title, const QString& description, QWidget* parent);
		};
	}
}

#endif
