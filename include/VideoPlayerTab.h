#ifndef __VIDEOPLAYERTAB_H__
#define __VIDEOPLAYERTAB_H__

#include "ui_VideoPlayer.h"
#include "Tab.h"

#include <QWidget>

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
