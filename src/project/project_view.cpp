#include "project_view.hpp"

#include <QUrl>
#include <QDebug>

using namespace Kiss::Project;

ProjectView::ProjectView(QWidget *parent)
: QTreeView(parent)
{
	setExpandsOnDoubleClick(true);
	setAcceptDrops(true);
	setDropIndicatorShown(true);
}

void ProjectView::dragEnterEvent(QDragEnterEvent *event)
{
	activateWindow();
	raise();
	event->accept();
}

void ProjectView::dragMoveEvent(QDragMoveEvent *event)
{
	const QModelIndex& index = indexAt(event->pos());
	if(index.isValid()) {
		if(index.parent() == QModelIndex()) setCurrentIndex(index);
		// TODO: This assumes a strictly two-level project model
		else setCurrentIndex(index.parent());
	}
	event->setDropAction(Qt::MoveAction);
	event->accept();
}

void ProjectView::dropEvent(QDropEvent *event)
{
	QStringList files;
	foreach(QUrl url, event->mimeData()->urls()) {
		if(url.isLocalFile()) files << url.toLocalFile();
	}

	emit filesDropped(files);
}