/********************************************************************
* Name      :  ProcUartMsgReq.cpp                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  Zhang Botong                                          
* Date      :  2018-06-13                                                 
********************************************************************/

#include "ProcUartMsgReq.h"
#include "MsgProcessor.h"

ProcUartMsgReq::ProcUartMsgReq():BaseReq( Requestion::ProcUartMsgReq )
{
	
}

int ProcUartMsgReq::processReq( Requestion *pReq )
{
	return MsgProcessor::getInstance()->processMsg();
}

ProcUartMsgReq::~ProcUartMsgReq()
{
	
}

