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

#include "AudioTutorial.h"

#include <QTextStream>
#include <QFile>

struct TutorialState
{
	TutorialState(const int& type, const QString& data, const QStringList& args)
		: type(type), data(data), args(args), mediaPlayer(0) {}

	~TutorialState() {
		if(mediaPlayer) delete mediaPlayer;
	}

	int type;
	QString data;
	QStringList args;
	Phonon::MediaObject* mediaPlayer;

	enum {
		Play,
		Wait
	};
};


AudioTutorial::AudioTutorial(const QString& path)
	: m_loaded(false), m_state(Off), m_currentIndex(0),
	m_current(0), m_path(path), m_exit(0)
{
	load();
}
AudioTutorial::~AudioTutorial() { delete m_exit; foreach(TutorialState* state, m_states) delete state; }

const bool AudioTutorial::start()
{
	if(!loaded()) return false;
	
	if(m_state != Off) stop();
	UiEventManager::ref().addListener(this);
	m_currentIndex = 0;
	
	resume();
	
	return true;
}

void AudioTutorial::resume()
{
	if(m_currentIndex >= m_states.size()) return;
	
	// qWarning() << m_currentIndex;
	TutorialState* current = m_states[m_currentIndex];
	m_current = current->mediaPlayer;
	
	if(current->type == TutorialState::Wait) {
		m_state = Waiting;
		m_current = 0;
	} else if(m_current && current->type == TutorialState::Play) {
		m_state = Playing;
		connect(m_current, SIGNAL(finished()), this, SLOT(finished()));
		m_current->play();
	}
}

void AudioTutorial::stop()
{
	m_state = Off;
	if(m_current) m_current->stop();
	UiEventManager::ref().removeListener(this);
}

const int AudioTutorial::state() const { return m_state; }
const bool AudioTutorial::loaded() const { return m_loaded; }

void AudioTutorial::event(const QString& name, const QStringList& args)
{
	
	if(m_states[m_currentIndex]->data != name) {
		if(m_state != Off && !fastForward(name)) {
			m_exit->mediaPlayer->play();
			stop();
			return;
		}
	}
	
	if(m_current) m_current->stop();
	
	++m_currentIndex;
	if(m_currentIndex < m_states.size()) resume();
	else stop();
}

void AudioTutorial::finished()
{
	++m_currentIndex;
	if(m_currentIndex < m_states.size()) resume();
	else stop();
}

void AudioTutorial::load()
{
	if(m_loaded) return;
	
	QFile file(m_path);
	if(!file.open(QIODevice::ReadOnly)) {
		qWarning() << "Failed to open" << m_path;
		return;
	} else {
		qWarning() << "Audio Tutorial created";
	}
	QTextStream stream(&file);
	QString lines = stream.readAll();
	file.close();
	
	qWarning() << lines;
	
	int i = 0;
	foreach(const QString& line, lines.split("\n")) {
		qWarning() << "Processing" << line;
		const QStringList& parts = line.split(" ");
		if(parts.size() != 2) {
			qWarning() << "line" << line << "in" << m_path << "is malformed.";
			continue;
		}
		TutorialState* state = 0;
		if(parts[0] == "play" || parts[0] == "exit") {
			state = new TutorialState(TutorialState::Play, parts[1], parts.mid(2));
			state->mediaPlayer = Phonon::createPlayer(Phonon::NoCategory, Phonon::MediaSource(state->data));
			if(state->mediaPlayer->errorType() != Phonon::NoError)
				qWarning() << "Error loading" << state->data << ":" << state->mediaPlayer->errorString();
		} else if(parts[0] == "wait") state = new TutorialState(TutorialState::Wait, parts[1], parts.mid(2));
		
		if(parts[0] == "exit") m_exit = state; else m_states.append(state);
	}
	
	m_loaded = true;
}

bool AudioTutorial::fastForward(const QString& name)
{
	for(; m_currentIndex < m_states.size(); ++m_currentIndex) {
		qWarning() << m_states[m_currentIndex]->data << name;
		if(m_states[m_currentIndex]->data == name) return true;
	}
	return false;
}