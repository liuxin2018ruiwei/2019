/********************************************************************
* Name      :  HeartBeatUCmd.cpp                                  
* Describ   :  get the heart beat values
* Author    :  liuxin                                        
* Date      :  2018-06-26                                                 
********************************************************************/
#include <stdio.h>
#include "HeartBeatUCmd.h"
#include "UartCommandType.h"
#include "HeartBeatInfo.h"
#include "SendHeartBeatReq.h"
#include "LittleEndianIO.h"


HeartBeatUCmd::HeartBeatUCmd():UartCommand( UartCommandCode::QUERY_HEART_BEAT ) 
{
	
}

int HeartBeatUCmd::processMsg(Message * pMsg)
{
	if(NULL == pMsg)
	{
		return -1;
	}

	UINT16 commandCode= pMsg->getCode();//Command Code
	UINT8  flags = pMsg->getFlags(); //Flags
	UINT8  sequenceNumber = pMsg->getSequenceNumber(); //Sequence Number
	UINT8* p = (UINT8 *)(pMsg->getData());


	
	if(  (flags  == UART_HEART_BEAT_FLAG_REC_BIT) && ((flags & 0x01) == 0x00) )
	{
		//printf("TEST get heat beat ok\n");
		return 0;
	}
	
	//
	return 0;
}
HeartBeatUCmd::~HeartBeatUCmd()
{
}
