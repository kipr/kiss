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

#ifndef _AUDIOTUTORIAL_H_
#define _AUDIOTUTORIAL_H_

#include "UiEventManager.h"

#include <QList>
#include <phonon>

struct TutorialState;

class AudioTutorial : public QObject, public UiEventListener
{
Q_OBJECT
public:
	AudioTutorial(const QString& path);
	~AudioTutorial();
	
	const bool start();
	void resume();
	void stop();
	
	const int state() const;
	const bool loaded() const;
	
	void event(const QString& name, const QStringList& args);

	enum {
		Playing,
		Waiting,
		Off
	};

private slots:
	void finished();

private:
	void load();
	bool m_loaded;
	
	bool fastForward(const QString& name);
	
	int m_state;
	int m_currentIndex;
	
	Phonon::MediaObject* m_current;
	QString m_path;
	
	TutorialState* m_exit;
	
	QList<TutorialState*> m_states;
};

#endif
