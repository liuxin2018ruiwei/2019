#ifndef OBJECT_OF_OBSERVER_H
#define OBJECT_OF_OBSERVER_H

#include"Observer.h"

class ObjectOfObserver
{
public:
	ObjectOfObserver();
	virtual ~ObjectOfObserver();
	//  pObver (in) the pointer of observer to add,and then ,when received data,the observer will be notify
	virtual int addObserver(Observer* pObver );
	// pObver (in) the pointer of observer to remove, when the observer obj was destoryed, it must be remove first
	virtual int removerObverser(Observer* pObver );
	// pData (in) the notify info 's pointer
	// return (out) the time which accept the data, 0 means nobody accept the data
	virtual int notifyObserver(unsigned char* pData);
private:
	// save the observer to notify in the future
	ObsverList m_Obsvers;
};

#endif
