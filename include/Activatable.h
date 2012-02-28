#ifndef _ACTIVATABLE_H_
#define _ACTIVATABLE_H_

#include <QObject>

template<typename T>
class Activatable
{
public:
	Activatable() : m_active(0) {}
	
	void setActive(T* active) { m_active = active; if(active) activated(); else deactivated(); }
	T* active() { return m_active; }
	bool isActive() { return m_active; }
	
protected:
	virtual void activated() {}
	virtual void deactivated() {}
	
private:
	T* m_active;
};

class ActivatableObject : public Activatable<QObject> {};

#endif
