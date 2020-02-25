#ifndef OBSERVER_H
#define OBSERVER_H

#include<list>

class Observer
{
public:
	Observer();
	virtual ~Observer();
	// pData (in) the notify info will be contained in the space of pData return OK 0; NG -1;
	virtual int update(void* pData);

	static const int UPDATE_OK;
	static const int UPDATE_NG;
	static const int UPDATE_BUSY;
};

typedef  std::list< Observer* > ObsverList;
#endif

