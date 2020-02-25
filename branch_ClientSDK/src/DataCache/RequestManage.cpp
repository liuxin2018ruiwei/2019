/********************************************************************
* Name      :  RequestManage.cpp                                  
* Describ   :  the defination of the class
* Author    :  Zhang Botong                                          
* Date      :  2018-06-08                                                 
********************************************************************/

#include "RequestManage.h"
#include "PthreadManage.h"

RequestManage * RequestManage::m_this = nullptr;

// delete a requestion from the unique
int RequestManage::deleteRequest( Requestion * pReq )
{
	int nRet = 0;
	Requestion::Priority pro = pReq->getPriority();
	if( Requestion::HighPriority == pro )
	{
		RecoLockGuard autolock( m_hpListlock );
		nRet = deleteRequestFromList( m_highProList, pReq );
	}
	else if( Requestion::MiddlePriority == pro )
	{
		RecoLockGuard autolock( m_mpListlock);
		nRet = deleteRequestFromList( m_middleProList, pReq );
	}
	else if( Requestion::LowPriority == pro )
	{
		RecoLockGuard autolock( m_lpListlock);
		nRet = deleteRequestFromList( m_lowProList, pReq );
	}
	else
	{
		// NOP
	}


	return nRet;
	
}

int RequestManage::deleteRequestFromList( ReqList &proList, Requestion *pReq )
{
	int nRet =  -1;
	ReqItr itr = proList.begin();
	while( itr != proList.end() )
	{
		if( *itr == pReq )
		{
			proList.erase( itr );
			nRet = 0;
			break;
		}
		
		itr++;
	}
	
	return nRet;
}


RequestManage * RequestManage::getInstance()
{
	if( nullptr == m_this )
	{
		m_this = new RequestManage;
	}

	return m_this;
}

// set a requestion to the unique
int RequestManage::setRequest( Requestion *pReq )
{
	if( nullptr == pReq )
	{
		return -1;
	}

	Requestion::Priority pro = pReq->getPriority();
	if( Requestion::HighPriority == pro )
	{
		RecoLockGuard autolock( m_hpListlock );
		m_highProList.push_back( pReq );
	}
	else if( Requestion::MiddlePriority == pro )
	{
		RecoLockGuard autolock( m_mpListlock);
		m_middleProList.push_back( pReq );
	}
	else if( Requestion::LowPriority == pro )
	{
		RecoLockGuard autolock( m_lpListlock);
		m_lowProList.push_back( pReq );
	}
	else
	{
		// NOP
	}

	PthreadManage::getInstance()->setReqSemPost();
	
	return 0;
}

// get a requestion from the unique of designated priority 
Requestion * RequestManage::getRequest(Requestion::Priority pro )
{
	Requestion *pReq  = nullptr;

	if( Requestion::HighPriority == pro )
	{
		RecoLockGuard autolock( m_hpListlock );
		if( !m_highProList.empty())
		{
			pReq = *m_highProList.begin();
			//m_highProList.pop_front( );
		}
	}
	else if( Requestion::MiddlePriority == pro )
	{
		RecoLockGuard autolock( m_mpListlock);
		if( !m_middleProList.empty() )
		{
			pReq = *m_middleProList.begin();
			//m_middleProList.pop_front( );
		}
	}
	else if( Requestion::LowPriority == pro )
	{
		RecoLockGuard autolock( m_lpListlock);
		if( !m_lowProList.empty() )
		{
			pReq = *m_lowProList.begin();
			//	m_lowProList.pop_front( );
		}
	}
	else
	{
		// NOP
	}
	
	return 0;
}


// get the requestion of highest priority from all of requestion unique
Requestion * RequestManage::getTopPriorityRequest()
{
	Requestion *pReq  = nullptr;
	Requestion *pReqTmp = nullptr;

	{// the '{}" is necessary
		RecoLockGuard autolock( m_hpListlock );
		if( !m_highProList.empty())
		{
			pReq = *m_highProList.begin();
			//m_highProList.pop_front( );
		}
	}

	{
		RecoLockGuard autolock( m_mpListlock);
		if( !m_middleProList.empty() )
		{
		    pReqTmp = *m_middleProList.begin();
			//m_middleProList.pop_front( );
		}
	}
	
	if( ( nullptr == pReq ) && ( nullptr != pReqTmp ) )
	{
		pReq = pReqTmp;
	}
	else if( ( nullptr != pReq ) && (nullptr != pReqTmp)
			 &&(pReq->getTimer() >  pReqTmp->getTimer() )
			 &&( ( pReq->getTimer() - pReqTmp->getTimer() ) > RequestTimeOut ) )
	{
		// some requestion which has waited for a long time,
		//should be set higher priority 

		pReq = pReqTmp;
		pReqTmp = nullptr;
	}
	else
	{
		// NOP
	}
	
	{
		RecoLockGuard autolock( m_lpListlock);
		if( !m_lowProList.empty() )
		{
			pReqTmp = *m_lowProList.begin();
			//	m_lowProList.pop_front( );
		}
	}

	if( ( nullptr == pReq ) && ( nullptr != pReqTmp ) )
	{
		pReq = pReqTmp;
	}
	else if( ( nullptr != pReq ) && (nullptr != pReqTmp)
			 &&(pReq->getTimer() >  pReqTmp->getTimer() )
			 &&( ( pReq->getTimer() - pReqTmp->getTimer() ) > RequestTimeOut ) )
	{
		pReq = pReqTmp;
		pReqTmp = nullptr;
	}
	else
	{
		// NOP
	}

	return pReq;

}

  
RequestManage::~RequestManage()
{
}


RequestManage::RequestManage()
{
}


