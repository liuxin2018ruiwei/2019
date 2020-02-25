#include "RecoLock.h"




//动态方式初始化互斥锁
RecoLock::RecoLock()
{
	pthread_mutex_init(&_mutex, NULL);
}

//注销互斥锁
RecoLock::~RecoLock()
{
	pthread_mutex_destroy(&_mutex);
}

//确保拥有互斥锁的线程对被保护资源的独自访问
void RecoLock::lock() const
{
	pthread_mutex_lock(&_mutex);
}

//释放当前线程拥有的锁，以使其它线程可以拥有互斥锁，对被保护资源进行访问
void RecoLock::unlock() const
{
	pthread_mutex_unlock(&_mutex);
}



//利用C++特性，进行自动加锁
RecoLockGuard::RecoLockGuard(const RecoLock& m) 
	: _lock(m)
{
	_lock.lock();
}

//利用C++特性，进行自动解锁
RecoLockGuard::~RecoLockGuard()
{
	_lock.unlock();
}


