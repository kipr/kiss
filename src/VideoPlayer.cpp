#include "VideoPlayer.h"
#include "MainWindow.h"

#include <MediaSource>
#include <MediaObject>
#include <VideoPlayer>

VideoPlayer::VideoPlayer(QWidget* parent) : QWidget(parent)
{
	setupUi(this);
	actionPlay->setEnabled(true);
	actionPause->setEnabled(false);
}

void VideoPlayer::activate() { MainWindow::ref().hideErrors(); }

void VideoPlayer::addActionsFile(QMenu*) {}
void VideoPlayer::addActionsEdit(QMenu*) {}
void VideoPlayer::addActionsHelp(QMenu*) {}

void VideoPlayer::addOtherActions(QMenuBar* menuBar)
{
	QMenu* video = menuBar->addMenu(tr("Video"));
	video->addAction(actionPlay);
	video->addAction(actionPause);
}

void VideoPlayer::addToolbarActions(QToolBar* toolbar)
{
	toolbar->addAction(actionPlay);
	toolbar->addAction(actionPause);
}

bool VideoPlayer::beginSetup() { return true; }
void VideoPlayer::completeSetup() { MainWindow::ref().setTabName(this, "Video Player"); }
bool VideoPlayer::close() { return true; }

void VideoPlayer::load(const QString& file) 
{
	ui_video->load(Phonon::MediaSource(file));
	ui_seek->setMediaObject(ui_video->mediaObject());
	ui_volume->setAudioOutput(ui_video->audioOutput());
	connect(ui_video->mediaObject(), SIGNAL(stateChanged(Phonon::State, Phonon::State)), 
		this, SLOT(stateChange(Phonon::State, Phonon::State)));
}

void VideoPlayer::stateChange(Phonon::State state, Phonon::State)
{
	actionPlay->setEnabled(state != Phonon::PlayingState);
	actionPause->setEnabled(state == Phonon::PlayingState);
}