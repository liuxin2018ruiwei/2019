/********************************************************************
* Name      :  UartMessageFilter.cpp                                  
* Describ   :  the difination of the class
* Author    :  Zhang Botong                                          
* Date      :  2018-06-15                                                 
********************************************************************/
#include <stdio.h>
#include "UartMessageFilter.h"
#include "QueryInfoUCmd.h"
#include "MessageQueue.h"
#include "HeartBeatUCmd.h"
#include "CommunicationAgent.h"
#include "Requestion.h"


UartMessageFilter::UartMessageFilter()
{
	
}


bool UartMessageFilter::filter(Message* mes)
{
	//PthreadManage::getInstance()->setReqSemPost();
	int nRet = 0;
	Requestion *rq = new Requestion(Requestion::ProcUartMsgReq);
	nRet |=  CommunicationAgent::getInstance()->sendReq( rq );
	if(nRet != 0)
	{
		printf("communication Err! nRet = %d \n",nRet);
		return -1;
	}

	return false;
}

bool UartMessageFilter::isExistCmdType( unsigned int cmdType,unsigned int cmdCode)
{
	return false;
}


UartMessageFilter::~UartMessageFilter()
{
	
}
