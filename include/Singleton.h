#ifndef __SINGLETON_H__
#define __SINGLETON_H__

template<typename T>
class Singleton
{
public:
	static T& ref() {
		static T instance;
		return instance;
	}
	
protected:
	Singleton() {}
	Singleton(const Singleton&) {}
	~Singleton() {}
};

#endif
