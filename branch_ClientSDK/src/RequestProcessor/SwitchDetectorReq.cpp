/********************************************************************
* Name      :  SwitchDetectorReq.cpp                                  
* Describ   :  to stop or start face ckeck by uart
* Author    :  liuxin                                          
* Date      :  2018-06-2                                               
********************************************************************/

#include "SwitchDetectorReq.h"
#include <stdio.h>
#include "UDPProtocol.h"
#include "CommunicationAgent.h"
#include "HeartBeatInfo.h"
#include "LittleEndianIO.h"
#include "SwitchRequestion.h"


typedef unsigned char UINT8;
typedef unsigned short int UINT16;

SwitchDetectorReq::SwitchDetectorReq():
	BaseReq( Requestion::SendSwitchDetectorReq )
{
}


int SwitchDetectorReq::sendStartFaceCheckByUart()
{
	UINT8 sendData[UART_FACE_CHECK_SEND_LEN] ={0};

	short int retCode = 0;

	LittleEndianIO::writeInt16((char *)sendData,UART_START_FACE_CHECK_COMMAND);//Command Code
	sendData[UART_FACE_CHECK_FLAG_LOCATION] = UART_FACE_CHECK_FLAG_SEND_BIT;	//Flags
	sendData[UART_FACE_CHECK_SEQUENCE_NUMBER_LOCATION] = 0; 			//Sequence Number

	HeartBeatInfo::getInstance()->getNowTimeSetLastConTime();
	
	return CommunicationAgent::getInstance()->sendMsg(0,UART_START_FACE_CHECK_COMMAND,
								 0, NULL, 0, CommunicationAgent::SendToOuter);
}


int SwitchDetectorReq::sendStopFaceCheckByUart()
{
	UINT8 sendData[UART_FACE_CHECK_SEND_LEN] ={0};

	short int retCode = 0;

	LittleEndianIO::writeInt16((char *)sendData,UART_STOP_FACE_CHECK_COMMAND);//Command Code
	sendData[UART_FACE_CHECK_FLAG_LOCATION] = UART_FACE_CHECK_FLAG_SEND_BIT;	//Flags
	sendData[UART_FACE_CHECK_SEQUENCE_NUMBER_LOCATION] = 0; 			//Sequence Number

	HeartBeatInfo::getInstance()->getNowTimeSetLastConTime();
	
	return CommunicationAgent::getInstance()->sendMsg(0,UART_STOP_FACE_CHECK_COMMAND,
								 0, NULL, 0, CommunicationAgent::SendToOuter);
}

#define START_FACE_CHECK (1)
#define STOP_FACE_CHECK (0)
int SwitchDetectorReq::processReq( Requestion *pReq )
{
	SwitchRequestion *p = (SwitchRequestion *)pReq;
	if(p->getUserFlag() == START_FACE_CHECK)
	{
		return sendStartFaceCheckByUart();
	}
	else
	{
		return sendStopFaceCheckByUart();
	}
	
}

SwitchDetectorReq::~SwitchDetectorReq()
{
	
}

