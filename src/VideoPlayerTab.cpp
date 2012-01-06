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

#include "VideoPlayerTab.h"
#include "MainWindow.h"
#include "MessageDialog.h"

#include <MediaSource>
#include <MediaObject>
#include <VideoPlayer>

#include <QMessageBox>

VideoPlayerTab::VideoPlayerTab(QWidget* parent) : QWidget(parent)
{
	setupUi(this);
	actionPlay->setEnabled(false);
	actionPause->setEnabled(false);
}

void VideoPlayerTab::activate() { MainWindow::ref().hideErrors(); }

void VideoPlayerTab::addActionsFile(QMenu*) {}
void VideoPlayerTab::addActionsEdit(QMenu*) {}
void VideoPlayerTab::addActionsHelp(QMenu*) {}

void VideoPlayerTab::addOtherActions(QMenuBar* menuBar)
{
	QMenu* video = menuBar->addMenu(tr("Video"));
	video->addAction(actionPlay);
	video->addAction(actionPause);
}

void VideoPlayerTab::addToolbarActions(QToolBar* toolbar)
{
	toolbar->addAction(actionPlay);
	toolbar->addAction(actionPause);
}

bool VideoPlayerTab::beginSetup() { return true; }
void VideoPlayerTab::completeSetup() { MainWindow::ref().setTabName(this, tr("Video Player")); }
bool VideoPlayerTab::close() { return true; }

void VideoPlayerTab::load(const QString& file) 
{
	ui_video->load(Phonon::MediaSource(file));
	ui_seek->setMediaObject(ui_video->mediaObject());
	ui_volume->setAudioOutput(ui_video->audioOutput());
	connect(ui_video->mediaObject(), SIGNAL(stateChanged(Phonon::State, Phonon::State)), 
		this, SLOT(stateChange(Phonon::State, Phonon::State)));
	ui_video->mediaObject()->play();
	if(ui_video->mediaObject()->errorType() != Phonon::NoError) {
		MessageDialog::showError(0, "simple_error", QStringList() << 
			QString("Unable to load ") + file <<
			ui_video->mediaObject()->errorString());
	}
}

void VideoPlayerTab::stateChange(Phonon::State state, Phonon::State)
{
	actionPlay->setEnabled(state != Phonon::PlayingState);
	actionPause->setEnabled(state == Phonon::PlayingState);
}