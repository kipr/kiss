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

#include "web_tab_menu.hpp"

#ifdef BUILD_WEB_TAB

#include <QDebug>
#include <QWebHistory>
#include <QWebView>

using namespace Kiss::Menu;

WebTabMenu::WebTabMenu()
	: ConcreteMenuable(menuName()),
	ActivatableObject()
{
	m_edit.append(node(activeAction("copy", "Copy", QKeySequence::Copy, this, "copy")));
	m_edit.append(node(activeAction("cut", "Cut", QKeySequence::Cut, this, "cut")));
	m_edit.append(node(activeAction("paste", "Paste", QKeySequence::Paste, this, "paste")));
	m_edit.append(Node::separator());
	m_edit.append(node(zoomIn = action("Zoom In", QKeySequence::ZoomIn)));
	m_edit.append(node(zoomOut = action("Zoom Out", QKeySequence::ZoomOut)));
	m_edit.append(node(resetZoom = action("Reset Zoom", QKeySequence("Ctrl+0"))));
	m_edit.append(Node::separator());
	m_edit.append(node(activeAction("find", "Find", QKeySequence::Find, this, "find")));
	
	
	back = node(activeAction("arrow_left", "Back", QKeySequence::Back, this, "back"));
	forward = node(activeAction("arrow_right", "Forward", QKeySequence::Forward, this, "forward"));
	reload = node(activeAction("arrow_refresh", "Refresh", QKeySequence::Refresh, this, "refresh"));
	open = node(activeAction("resultset_next", "Open in Browser", QKeySequence::UnknownKey, this, "openInBrowser"));
	
	back->hideOnDisable = forward->hideOnDisable = false;
	
	m_toolbar.append(back);
	m_toolbar.append(forward);
	m_toolbar.append(reload);
	m_toolbar.append(open);
	m_toolbar.append(Node::separator());
}

void WebTabMenu::activated()
{
	ActivatableObject::activated();
	menuManager()->addActivation(this);
}

void WebTabMenu::deactivated()
{
	ActivatableObject::deactivated();
	menuManager()->removeActivation(this);
}

void WebTabMenu::triggered()
{
	Widget::WebTab *web = dynamic_cast<Widget::WebTab *>(active());
	if(!web) return;
	
	QAction *_ = static_cast<QAction *>(sender());
	
	QWebView *webView = web->webView();
	if(_ == zoomIn) webView->setZoomFactor(webView->zoomFactor() * 1.1);
	else if(_ == zoomOut) webView->setZoomFactor(webView->zoomFactor() * 0.9);
	else if(_ == resetZoom) webView->setZoomFactor(1.0);
}

void WebTabMenu::update()
{
	Widget::WebTab *web = dynamic_cast<Widget::WebTab *>(active());
	if(!web) return;
	
	back->rawAction->setEnabled(web->history()->canGoBack());
	forward->rawAction->setEnabled(web->history()->canGoForward());
}

QString WebTabMenu::menuName()
{
	return "Web";
}

#endif