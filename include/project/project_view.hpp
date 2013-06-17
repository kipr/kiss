#ifndef _PROJECT_VIEW_HPP_
#define _PROJECT_VIEW_HPP_

#include <QTreeView>
#include <QDragEnterEvent>

#include "project_model.hpp"

namespace Kiss
{
	namespace Project
	{
		class ProjectView : public QTreeView
		{
			Q_OBJECT
		public:
			ProjectView(QWidget* parent = 0);
			void setModel(Model* model);
		protected:
			void dragEnterEvent(QDragEnterEvent* event);
			void dragMoveEvent(QDragMoveEvent* event);
			void dropEvent(QDropEvent *event);
		signals:
			void filesDraggedOn(QModelIndex index);
			void filesDropped(QStringList files);
		private:
			Model* m_model;
		};
	}
}

#endif