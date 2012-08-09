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

#ifndef __TABBEDWIDGET_H__
#define __TABBEDWIDGET_H__

class MainWindow;
class QMenu;
class QMenuBar;
class QToolBar;
class Project;
class TinyNode;

#include <QString>
#include <QWidget>
#include <QVariant>
#include <QDebug>
#include <QFileInfo>

#define TABBED_WIDGET_PROPERTY "_kiss_tabbed_"

class TabbedWidget
{
public:
	TabbedWidget(QWidget* widget, MainWindow* mainWindow = 0) : m_widget(widget), m_mainWindow(mainWindow),
	 	m_associatedProject(0), m_associatedFile("") {}
	~TabbedWidget() {}
	
	/*!
	 * Called every time this Tab is switched to. Useful for setting window title.
	 */
	virtual void activate() { qWarning() << "Activated"; }
	
	/*!
	 * Begin setup is called before a Tab is added to the Tab Widget. 
	 * \return false cancels adding this Tab, true continues setup process
	 */
	virtual bool beginSetup() = 0;
	
	/*!
	 * The Tab has been added to the Tab Widget. Set the Tab Name here.
	 */
	virtual void completeSetup() = 0;
	
	/*!
	 * Requests a Tab to close
	 * \return false cancels the close operation, true closes the Tab
	 */
	virtual bool close() = 0;
	
	virtual void refreshSettings() = 0;
	
	QWidget* widget() const { return m_widget; }
	
	MainWindow* mainWindow() const { return m_mainWindow; }
	void setMainWindow(MainWindow* mainWindow) { m_mainWindow = mainWindow; }
	
	void setAssociatedFile(const QString& associatedFile) { m_associatedFile = associatedFile; }
	const QString& associatedFile() const { return m_associatedFile; }
	QFileInfo associatedFileInfo() const { return QFileInfo(m_associatedFile); }
	QString associatedFileName() { return associatedFileInfo().fileName(); }
	QString associatedFileBaseName() { return associatedFileInfo().baseName(); }
	QString associatedFileSuffix() { return associatedFileInfo().completeSuffix(); }
	bool isFileAssociated() const { return !m_associatedFile.isEmpty(); }
	
	void setAssociatedProject(Project* associatedProject) { m_associatedProject = associatedProject; }
	Project* associatedProject() const { return m_associatedProject; }
	bool isProjectAssociated() const { return m_associatedProject; }
	
private:
	QWidget* m_widget;
	MainWindow* m_mainWindow;
	
	Project* m_associatedProject;
	QString m_associatedFile;
};

#endif
