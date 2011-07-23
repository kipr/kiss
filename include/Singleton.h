#ifndef __SINGLETON_H__
#define __SINGLETON_H__

/*! \class Singleton
 * \brief Implements a simple Singleton Pattern
 */
template<typename T>
class Singleton
{
public:
	/*! \return Reference to singleton object */
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
