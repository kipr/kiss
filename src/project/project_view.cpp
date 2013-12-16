#include "project_view.hpp"

using namespace kiss::project;

ProjectView::ProjectView(QWidget *parent)
: QTreeView(parent)
{
	setExpandsOnDoubleClick(true);
	setAcceptDrops(true);
	setDragDropMode(QAbstractItemView::DragDrop);
	setDropIndicatorShown(true);
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
