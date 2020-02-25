/********************************************************************
* Name      :  Requestion.cpp                                  
* Describ   :  the defination of the class
* Author    :  Zhang Botong                                          
* Date      :  2018-06-11                                                 
********************************************************************/
#include <sys/time.h>
#include "Requestion.h"

Requestion::Requestion( ReqID id): m_id( id ), m_timer( 0),
								   m_pro( MiddlePriority ),
								   m_state( 0 ),m_dstID( 0)
{
	struct timeval tv;
	gettimeofday( &tv, nullptr );
	m_timer = tv.tv_sec * 1000000 + tv.tv_usec;
}
Requestion::Requestion( ReqID id,  Priority pro): m_id( id ),
												  m_timer( 0),
												  m_pro( pro ),
												  m_state( 0 ),
												  m_dstID( 0)
												  
{
	struct timeval tv;
	gettimeofday( &tv, nullptr );
	m_timer = tv.tv_sec * 1000000 + tv.tv_usec;	
}

Requestion::ReqID Requestion::getRequestID()
{
	return m_id;
}

UINT32 Requestion::getRequestState()
{
	return m_state;
}

UINT32 Requestion::setRequestState( UINT32 state )
{
	m_state = state;
	return m_state;
}

Requestion::Priority Requestion::getPriority()
{
	return m_pro;
}

Requestion::Priority Requestion::setPriority( Priority pro )
{
	m_pro = pro;
	return m_pro;
}

UINT32 Requestion::getDestination()
{
	return m_dstID;
}

int Requestion::setDestination( UINT32 dstID)
{
	m_dstID = dstID;

	return m_dstID;
}

UINT64 Requestion::getTimer()
{
	return m_timer;
}

Requestion::~Requestion()
{
}
