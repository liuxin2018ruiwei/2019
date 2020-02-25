/********************************************************************
* Name      :  SendHeartBeatReq.cpp                                  
* Describ   :  the definaition of the class
* Author    :  liuxin                                          
* Date      :  2018-06-25                                                
********************************************************************/

#include "SendHeartBeatReq.h"
#include <stdio.h>
#include "UDPProtocol.h"
#include "CommunicationAgent.h"
#include "HeartBeatInfo.h"
#include "LittleEndianIO.h"


typedef unsigned char UINT8;
typedef unsigned short int UINT16;



SendHeartBeatReq::SendHeartBeatReq():
	BaseReq( Requestion::SendHeartBeatConReq )
{
	//m_sequenceNumber = 0;
}





int SendHeartBeatReq::sendHeatBeatByUart()
{
	UINT8 sendData[UART_HEART_BEAT_SEND_LEN] ={0};

	short int retCode = 0;

	LittleEndianIO::writeInt16((char *)sendData,UART_HEART_BEAT_COMMAND);//Command Code
	sendData[UART_HEART_BEAT_FLAG_LOCATION] = UART_HEART_BEAT_FLAG_SEND_BIT;	//Flags
	sendData[UART_HEART_BEAT_SEQUENCE_NUMBER_LOCATION] = 0; 			//Sequence Number

	 //test
	/*UINT8 *p = sendData;
	for(int i = 0 ; i < UART_HEART_BEAT_SEND_LEN; i++)
	{
		printf("%x ", *p);
		p++;
	}
	printf("\n");
	*/

	HeartBeatInfo::getInstance()->getNowTimeSetLastConTime();
	
	//printf("sendMsg ok\n");
	return CommunicationAgent::getInstance()->sendMsg(0,UART_HEART_BEAT_COMMAND,
								 0, NULL, 0, CommunicationAgent::SendToOuter);
}

int SendHeartBeatReq::processReq( Requestion *pReq )
{
	return sendHeatBeatByUart();
}

SendHeartBeatReq::~SendHeartBeatReq()
{
	
}

