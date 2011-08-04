/**************************************************************************
 *  Copyright 2007-2011 KISS Institute for Practical Robotics             *
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

#ifndef __VIDEOPLAYERTAB_H__
#define __VIDEOPLAYERTAB_H__

#include "ui_VideoPlayer.h"
#include "Tab.h"

#include <QWidget>
#include <Phonon>

class VideoPlayerTab : public QWidget, public Tab, public Ui::VideoPlayer
{
	Q_OBJECT
public:
	VideoPlayerTab(QWidget* parent = 0);
	
	void activate();
	
	void addActionsFile(QMenu* file);
	void addActionsEdit(QMenu* edit);
	void addActionsHelp(QMenu* help);
	void addOtherActions(QMenuBar* menuBar);
	void addToolbarActions(QToolBar* toolbar);
	
	bool beginSetup();
	virtual void completeSetup();
	
	bool close();
	
	void refreshSettings() {}
	
	void load(const QString& file);
	
private slots:
	void stateChange(Phonon::State state, Phonon::State oldState);
};

#endif
