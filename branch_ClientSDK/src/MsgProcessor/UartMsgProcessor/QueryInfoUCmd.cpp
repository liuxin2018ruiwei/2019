/********************************************************************
* Name      :  QueryInfoUCmd.cpp                                  
* Describ   :  the defination of the class
* Author    :  Zhang Botong                                          
* Date      :  2018-06-15                                                 
********************************************************************/

#include "QueryInfoUCmd.h"
#include "UartCommandType.h"

#include "LittleEndianIO.h"
#include <cstring>
#include "ConfigManage.h"
#include <stdio.h>


QueryInfoUCmd::QueryInfoUCmd():UartCommand( UartCommandCode::QUERY_MODULE_INFO ) 
{
}

int QueryInfoUCmd::processMsg(Message * pMsg)
{
	if(NULL == pMsg)
	{
		return -1;
	}

		
	UINT16 commandCode= pMsg->getCode();//Command Code
	UINT8  flags = pMsg->getFlags(); //Flags
	UINT8  sequenceNumber = pMsg->getSequenceNumber(); //Sequence Number
	UINT8* p = (UINT8 *)(pMsg->getData());

	//printf("get ok!!processMsg commandCode = %x\n\n\n\n\n",commandCode);
	
	char protocolVersion[PROTOCOL_VERSION_LEN]= {0};
	memcpy(protocolVersion,p,PROTOCOL_VERSION_LEN);


	
	
	if( (flags == UART_GET_MODULE_INFO_FLAG_REC_BIT) && ((flags & 0x01) == 0x00) )
	{
		ConfigManage::getInstance()->setModuleProtocolVersion(protocolVersion,PROTOCOL_VERSION_LEN);
	
		return 0;
	}

	return 0;
}
QueryInfoUCmd::~QueryInfoUCmd()
{
}
