/********************************************************************
* Name      :  BaseReq.cpp                                  
* Describ   :  the defination of the class
* Author    :  Zhang Botong                                          
* Date      :  2018-06-12                                                 
********************************************************************/

#include "BaseReq.h"

BaseReq::BaseReq( Requestion::ReqID id):m_id( id )
{

}

int BaseReq::processReq( Requestion *pReq )
{
	return 0;
}

Requestion::ReqID BaseReq::getReqID()
{
	return m_id;
}

BaseReq::~BaseReq()
{
}
