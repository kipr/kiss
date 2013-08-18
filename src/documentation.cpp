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

using namespace kiss;
using namespace kiss::widget;

Documentation::Documentation(MainWindow *parent)
    : Tab(new QListWidget(parent), parent)
{
}

void Documentation::activate()
{
    QList<menu::Menuable *> menus = mainWindow()->menuablesExcept(mainWindow()->standardMenus());
    foreach(menu::Menuable *menu, menus) {
        ActivatableObject *activatable = dynamic_cast<ActivatableObject *>(menu);
        if(activatable) activatable->setActive(0);
    }
}

bool Documentation::beginSetup()
{
    QListWidget *list = qobject_cast<QListWidget *>(widget());
    const QIcon defaultIcon(ResourceHelper::ref().icon("text-x-generic"));
    Q_FOREACH(const DocumentationSource &d, DocumentationManager::ref().documentationSources()) {
        QListWidgetItem *item = new QListWidgetItem(list);
        const QString iconPath = d.metadata().value("icon");
        item->setIcon(iconPath.isEmpty() ? defaultIcon : QIcon(iconPath));
        item->setData(Qt::UserRole, d.metadata()["goto"]);
        item->setSizeHint(QSize(100, 65));
        list->setItemWidget(item, new TitleDescription(
            d.metadata()["name"],
            d.metadata()["description"],
            list));
    }
    list->setIconSize(QSize(50, 50));
    connect(list, SIGNAL(itemDoubleClicked(QListWidgetItem *)),
    SLOT(itemDoubleClicked(QListWidgetItem *)));

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

void Documentation::itemDoubleClicked(QListWidgetItem *item)
{
    const QString &location = item->data(Qt::UserRole).toString();
    if(location.endsWith("pdf")) {
        QDesktopServices::openUrl(QUrl::fromUserInput(location));
        return;
    }

    #ifdef Q_OS_WIN
    const QString &sysLocation = "file:///" + location;
    #else
    const QString &sysLocation = "file://" + location;
    #endif

    QDesktopServices::openUrl(QUrl::fromUserInput(sysLocation));
}

#endif

