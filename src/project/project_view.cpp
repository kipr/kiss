#include "project_view.hpp"

using namespace kiss::project;

ProjectView::ProjectView(QWidget *parent)
: QTreeView(parent)
{
	setAcceptDrops(true);
	setDragDropMode(QAbstractItemView::DragDrop);
	setDropIndicatorShown(true);
}

QModelIndex ProjectView::currentIndex() const
{
  return selectionModel()->currentIndex();
}

void ProjectView::dragEnterEvent(QDragEnterEvent *event)
{
	activateWindow();
	raise();
	event->acceptProposedAction();
}

void ProjectView::dragMoveEvent(QDragMoveEvent *event)
{
	const QModelIndex &index = indexAt(event->pos());
	if(index.isValid()) setCurrentIndex(index);
	event->acceptProposedAction();
}
