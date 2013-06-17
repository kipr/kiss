#include "project_view.hpp"

#include <QUrl>
#include <QDebug>

using namespace Kiss::Project;

ProjectView::ProjectView(QWidget* parent)
: QTreeView(parent)
{
	setAcceptDrops(true);
	setDropIndicatorShown(true);
}

void ProjectView::dragEnterEvent(QDragEnterEvent *event)
{
	event->accept();
}

void ProjectView::dragMoveEvent(QDragMoveEvent *event)
{
	event->setDropAction(Qt::MoveAction);
	event->accept();
}

void ProjectView::dropEvent(QDropEvent *event)
{
	QList<QUrl> urls = event->mimeData()->urls();
	QStringList files;
	foreach(QUrl url, urls)
	if(url.isLocalFile()) files << url.toLocalFile();

	emit filesDropped(files);
}