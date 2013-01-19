#include "documentation.hpp"

#ifdef BUILD_DOCUMENTATION_TAB

#include "main_window.hpp"
#include "resource_helper.hpp"
#include "title_description_widget.hpp"
#include "documentation_manager.hpp"

#include <QListWidget>
#include <QPushButton>
#include <QFileInfo>
#include <QDir>
#include <QDesktopServices>

#include "main_window_menu.hpp"

using namespace Kiss;
using namespace Kiss::Widget;

Documentation::Documentation(MainWindow *parent)
	: Tab(new QListWidget(parent), parent)
{
	QListWidget *list = qobject_cast<QListWidget *>(widget());
	//list->setAlternatingRowColors(true);
}

void Documentation::activate()
{
	QList<Menu::Menuable *> menus = mainWindow()->menuablesExcept(mainWindow()->standardMenus());
	foreach(Menu::Menuable *menu, menus) {
		ActivatableObject* activatable = dynamic_cast<ActivatableObject *>(menu);
		if(activatable) activatable->setActive(0);
	}
}

bool Documentation::beginSetup()
{
	QListWidget *list = qobject_cast<QListWidget*>(widget());
	const QString providedBy("Provided by ");
	foreach(const DocumentationLocation& location, DocumentationManager::ref().locations()) {
		QListWidgetItem* item = new QListWidgetItem(list);
		item->setIcon(ResourceHelper::ref().icon("text-x-generic"));
		item->setData(Qt::UserRole, location.location());
		item->setSizeHint(QSize(100, 65));
		list->setItemWidget(item,
			new TitleDescription(
				location.name(),
				location.description(),
				list)
			);
	}
	list->setIconSize(QSize(50, 50));
	connect(list, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
		SLOT(itemDoubleClicked(QListWidgetItem*)));
	
	return true;
}

void Documentation::completeSetup()
{
	mainWindow()->setTabName(widget(), tr("Documentation"));
}

bool Documentation::close()
{
	return true;
}

void Documentation::refreshSettings()
{
	
}

void Documentation::itemDoubleClicked(QListWidgetItem* item)
{
	const QString& location = item->data(Qt::UserRole).toString();
	if(location.endsWith("pdf")) {
		QDesktopServices::openUrl(QUrl::fromUserInput(location));
		return;
	}
	
#ifdef Q_OS_WIN
	const QString& sysLocation = "file:///" + location;
#else
	const QString& sysLocation = "file://" + location;
#endif

	QDesktopServices::openUrl(QUrl::fromUserInput(sysLocation));
}

#endif

