#ifndef _LISTENABLE_H_
#define _LISTENABLE_H_

#include <QList>

template<typename T>
struct Listenable
{
	void addListener(T* t) { m_listeners.append(t); }
	void removeListener(T* t) { m_listeners.removeAll(t); }
	void removeAllListeners() { m_listeners.clear(); }
	const QList<T*>& listeners() { return m_listeners; }
private:
	QList<T*> m_listeners;
};

#endif
