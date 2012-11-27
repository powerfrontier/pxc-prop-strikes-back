#ifndef _SINGLETON_H_
#define _SINGLETON_H_

template<class T>
class Singleton {
	public:
	static T* instancePtr() {
		static T* instance = new T();
		return instance;
	}

	static T& instance() {
		return *(instancePtr());
	}
};

#endif

