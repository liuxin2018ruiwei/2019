/********************************************************************
* Name      :  SwitchRequestion.cpp                                  
* Describ   :  the definaition of the class
* Author    :  Zhang Botong                                          
* Date      :  2018-06-11                                                 
********************************************************************/

#include "SwitchRequestion.h"

SwitchRequestion::SwitchRequestion( int flag, ReqID id ):
	Requestion( id ), m_flag( flag )
{
}

SwitchRequestion::SwitchRequestion( int flag, ReqID id, Priority pro ):Requestion( id, pro ), m_flag( flag )
{
}

UINT64 SwitchRequestion::getUserFlag()
{
	return m_flag;
}

SwitchRequestion::~SwitchRequestion()
{
	
}
