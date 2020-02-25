/********************************************************************
* Name      :  UserCommonRequestion.cpp                                  
* Describ   :  the definaition of the class
* Author    :  Zhang Botong                                          
* Date      :  2018-06-11                                                 
********************************************************************/

#include "UserCommonRequestion.h"

UserCommonRequestion::UserCommonRequestion( UINT64 usrReqID, ReqID id ):
	Requestion( id ), m_usrReqID( usrReqID )
{
}

UserCommonRequestion::UserCommonRequestion( UINT64 usrReqID, ReqID id, Priority pro ):Requestion( id, pro ), m_usrReqID( usrReqID )
{
}

UINT64 UserCommonRequestion::getUserReqID()
{
	return m_usrReqID;
}

UserCommonRequestion::~UserCommonRequestion()
{
	
}
