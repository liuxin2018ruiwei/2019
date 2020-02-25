#ifndef RECO_LOCK_H__
#define RECO_LOCK_H__


#include <pthread.h>




//互斥锁类
class RecoLock 
{
public:
	RecoLock();
	~RecoLock();

	virtual void lock() const;
	virtual void unlock() const;

private:
	mutable pthread_mutex_t _mutex;
};

//自动锁
class RecoLockGuard
{
public:
	RecoLockGuard(const RecoLock&);
	~RecoLockGuard();

private:
	const RecoLock& _lock;
};




#endif
