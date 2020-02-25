/********************************************************************
* Name      :  GetServiceVisionReq.cpp                                  
* Describ   :                                                        
* Author    :  Zhang Botong                                          
* Date      :  2018-06-08                                                 
********************************************************************/

#include <stdio.h>
#include "GetServiceVisionReq.h"
#include "CommunicationAgent.h"
#include "LittleEndianIO.h"

GetServiceVisionReq::GetServiceVisionReq():
	BaseReq( Requestion::GetServiceVisionReqID )
{

}

int GetServiceVisionReq::processReq( Requestion *pReq )
{
	//printf("sendMsg ->GetServiceVisionReq %d : %s\n", __LINE__, __func__);
	UINT8 sendData[UART_GET_MODULE_INFO_SEND_LEN] ={0};

	short int retCode = 0;

	LittleEndianIO::writeInt16((char *)sendData,UART_GET_MODULE_INFO_COMMAND);//Command Code
	sendData[UART_GET_MODLUE_INFO_FLAG_LOCATION] = UART_GET_FLAG_SEND_BIT;	//Flags
	sendData[UART_SEQUENCE_NUMBER_LOCATION] = 0;			//Sequence Number

	 //test
	/*UINT8 *p = sendData;
	for(int i = 0 ; i < UART_GET_MODULE_INFO_SEND_LEN; i++)
	{
		printf("%x ", *p);
		p++;
	}
	printf("\n");
	printf("sendMsg ok\n");
	*/
	
	return CommunicationAgent::getInstance()->sendMsg(0,UART_GET_MODULE_INFO_COMMAND,
								 0, NULL, 0, CommunicationAgent::SendToOuter);

	
	return 0;
}

GetServiceVisionReq::~GetServiceVisionReq()
{
	
}
