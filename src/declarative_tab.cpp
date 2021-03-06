/**************************************************************************
 *  Copyright 2007-2012 KISS Institute for Practical Robotics             *
 *                                                                        *
 *  This file is part of KISS (Kipr's Instructional Software System).     *
 *                                                                        *
 *  KISS is free software: you can redistribute it and/or modify          *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation, either version 2 of the License, or     *
 *  (at your option) any later version.                                   *
 *                                                                        *
 *  KISS is distributed in the hope that it will be useful,               *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *  GNU General Public License for more details.                          *
 *                                                                        *
 *  You should have received a copy of the GNU General Public License     *
 *  along with KISS.  Check the LICENSE file in the project root.         *
 *  If not, see <http://www.gnu.org/licenses/>.                           *
 **************************************************************************/

#include "declarative_tab.hpp"

#include <QDebug>
#include <QGraphicsObject>

#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <QDeclarativeError>
#include <QDeclarativeEngine>
#include <QDesktopServices>

#include "developer_menu.hpp"
#include "main_window.hpp"

using namespace kiss;
using namespace kiss::widget;

DeclarativeTools::DeclarativeTools(MainWindow *const mainWindow, DeclarativeTab *const tab)
	: _mainWindow(mainWindow)
  , _tab(tab)
{
}

void DeclarativeTools::newProject()
{
  _mainWindow->newProject();
}

void DeclarativeTools::open()
{
  _mainWindow->open();
}

void DeclarativeTools::openWeb(const QString &url)
{
  QDesktopServices::openUrl(QUrl::fromUserInput(url));
}

void DeclarativeTools::setClosable(const bool closable)
{
  // _tab->setClosable(closable);
}

void DeclarativeTools::settings()
{
  _mainWindow->settings();
}

const QStringList DeclarativeTools::templates(const QString &target)
{
  return QStringList();
}

void DeclarativeTools::tellActivated()
{
  emit activated();
}

void DeclarativeTools::tellCompletedSetup()
{
  emit completedSetup();
}

DeclarativeTab::DeclarativeTab(const QUrl &file, MainWindow *parent)
	: Tab(new QDeclarativeView(file), parent),
#ifdef BUILD_DEVELOPER_TOOLS
	m_watcher(this), 
#endif
	m_kiss(parent, this)
{
  QDeclarativeView *view = dynamic_cast<QDeclarativeView*>(widget());
  if(!view) return;
  QString color = "#ffffff";
  #ifndef Q_OS_WIN
  color = mainWindow()->palette().color(QPalette::Background).name().toUtf8();
  #endif
  view->rootContext()->setContextProperty("kissBackground", color);
  view->rootContext()->setContextProperty("kiss", &m_kiss);
  view->rootContext()->setContextProperty("tab", this);
  view->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
  view->setResizeMode(QDeclarativeView::SizeRootObjectToView);
#ifdef BUILD_DEVELOPER_TOOLS
  const QString &localFile = file.toLocalFile();
  if(!localFile.isEmpty()) {
    m_watcher.addPath(localFile);
    connect(&m_watcher, SIGNAL(fileChanged(const QString&)), this, SLOT(reload()));
  }
#endif
}

DeclarativeTab::~DeclarativeTab()
{
	delete widget();
}

void DeclarativeTab::activate()
{
	QList<menu::Menuable*> menus = mainWindow()->menuablesExcept(mainWindow()->standardMenus());
	foreach(menu::Menuable *menu, menus) {
		ActivatableObject *activatable = dynamic_cast<ActivatableObject*>(menu);
		if(activatable) activatable->setActive(0);
	}
	m_kiss.tellActivated();
}

bool DeclarativeTab::beginSetup()
{
	QDeclarativeView *view = dynamic_cast<QDeclarativeView*>(widget());
	return view && !view->errors().size();
}

void DeclarativeTab::completeSetup()
{
	m_kiss.tellCompletedSetup();
}

bool DeclarativeTab::close()
{
#ifdef BUILD_DEVELOPER_TOOLS
	QDeclarativeView *view = dynamic_cast<QDeclarativeView*>(widget());
	if(view) m_watcher.addPath(view->source().toLocalFile());
	disconnect(&m_watcher, SIGNAL(fileChanged(const QString&)));
#endif
	return true;
}

void DeclarativeTab::reload()
{
	QDeclarativeView *view = dynamic_cast<QDeclarativeView*>(widget());
	if(!view) return;
	
	QUrl source = view->source();
	view->engine()->clearComponentCache();
	view->setSource(QUrl());
	view->setSource(source);
	
	completeSetup();
}

void DeclarativeTab::setTitle(const QString &title)
{
	mainWindow()->setTabName(widget(), title);
}

void DeclarativeTab::refreshSettings()
{
	
}
