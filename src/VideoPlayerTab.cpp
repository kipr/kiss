#include "VideoPlayerTab.h"
#include "MainWindow.h"

#include <MediaSource>
#include <MediaObject>
#include <VideoPlayer>

VideoPlayerTab::VideoPlayerTab(QWidget* parent) : QWidget(parent)
{
	setupUi(this);
	actionPlay->setEnabled(true);
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
}

void VideoPlayerTab::stateChange(Phonon::State state, Phonon::State)
{
	actionPlay->setEnabled(state != Phonon::PlayingState);
	actionPause->setEnabled(state == Phonon::PlayingState);
}