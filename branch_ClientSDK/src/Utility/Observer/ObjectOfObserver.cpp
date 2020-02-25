#include <stdio.h>
#include "ObjectOfObserver.h"
//#include "../MsgResponse.h"
//#include"log.h"


ObjectOfObserver::ObjectOfObserver()
{
}

ObjectOfObserver::~ObjectOfObserver()
{
}

//  pObver (in) the pointer of observer to add,and then ,when received data,the observer will be notify
int ObjectOfObserver::addObserver(Observer* pObver)
{
	bool isExisted = false;
	//printf("ObjectOfObserver::addObserver start");
	ObsverList::iterator it = m_Obsvers.begin();
	for (; it != m_Obsvers.end(); it++) {
		if (*it == pObver) {
			isExisted = true;
			break;
		}
	}
	if (false == isExisted) {
//		printf("ObjectOfObserver::addObserver");
		// add the observer into uart notify list
		m_Obsvers.push_back(pObver);
	}
	return 0;
}

// pObver (in) the pointer of observer to remove, when the observer obj was destoryed, it must be remove first
int ObjectOfObserver::removerObverser(Observer* pObver)
{
	// find the Observer and delete it
	ObsverList::iterator it = m_Obsvers.begin();
	for (; it != m_Obsvers.end(); it++)
	{
		if (*it == pObver)
		{
			m_Obsvers.erase(it);
			break;
		}
	}

	return 0;
}


// pData (in) the notify info 's pointer
// return (out) the time which accept the data, 0 means nobody accept the data
int ObjectOfObserver::notifyObserver(unsigned char* pData)
{
    // notify all the observers, and count the times accept the data
    int nRet = -1;
	int update_result = 0;
    ObsverList::iterator it = m_Obsvers.begin();
    for (; it != m_Obsvers.end(); it++)
    {
		update_result = (*it)->update(pData);
        if(Observer::UPDATE_OK ==  update_result)
        {
            nRet = 0;
        }
		else
		{
			printf("update fail %d", update_result);
		}
    }

    return nRet;
}
