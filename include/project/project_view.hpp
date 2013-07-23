#ifndef _PROJECT_VIEW_HPP_
#define _PROJECT_VIEW_HPP_

#include <QTreeView>
#include <QDragEnterEvent>

#include "project_model.hpp"

namespace kiss
{
	namespace project
	{
		class ProjectView : public QTreeView
		{
			Q_OBJECT
		public:
			ProjectView(QWidget *parent = 0);
		protected:
			void dragEnterEvent(QDragEnterEvent *event);
			void dragMoveEvent(QDragMoveEvent *event);
			void dropEvent(QDropEvent *event);
		signals:
			void filesDropped(QStringList files);
		};
	}
}

#endif