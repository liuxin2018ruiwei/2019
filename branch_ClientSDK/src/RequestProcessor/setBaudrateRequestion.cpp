/********************************************************************
* Name      :  setBaudrateRequestion.cpp                                  
* Describ   :  the definaition of the class
* Author    :  Zhang Botong                                          
* Date      :  2018-06-11                                                 
********************************************************************/

#include "setBaudrateRequestion.h"

setBaudrateRequestion::setBaudrateRequestion( UINT32 setBaudrate, ReqID id ):
	Requestion( id ), m_baudrate( setBaudrate )
{
}

setBaudrateRequestion::setBaudrateRequestion( UINT32 setBaudrate, ReqID id, Priority pro ):Requestion( id, pro ), m_baudrate( setBaudrate )
{
}

UINT64 setBaudrateRequestion::getBaudRate()
{
	return m_baudrate;
}

setBaudrateRequestion::~setBaudrateRequestion()
{
	
}
