/********************************************************************
* Name      :  StartFaceCheckUCmd.cpp                                  
* Describ   :  
* Author    :  liuxin                                        
* Date      :  2018-06-26                                                 
********************************************************************/
#include <stdio.h>
#include "StartFaceCheckUCmd.h"
#include "UartCommandType.h"

#include "SwitchDetectorReq.h"

#include "LittleEndianIO.h"


StartFaceCheckUCmd::StartFaceCheckUCmd():UartCommand( UartCommandCode::QUERY_START_FACE_CHECK ) 
{
	
}

int StartFaceCheckUCmd::processMsg(Message * pMsg)
{
	if(NULL == pMsg)
	{
		return -1;
	}

	UINT16 commandCode= pMsg->getCode();//Command Code
	UINT8  flags = pMsg->getFlags(); //Flags
	UINT8  sequenceNumber = pMsg->getSequenceNumber(); //Sequence Number
	UINT8* p = (UINT8 *)(pMsg->getData());




	
	if(  (flags == UART_FACE_CHECK_FLAG_REC_BIT) && ((flags & 0x01)== 0x00) )
	{
		//HeartBeatInfo::getInstance()->getNowTimeSetLastReplyTime();
		//HeartBeatInfo::getInstance()->setHeatBeatLostCount(0);
	

		printf("ok");
		return 0;
	}
	
	//
	return 0;
}
StartFaceCheckUCmd::~StartFaceCheckUCmd()
{
}
