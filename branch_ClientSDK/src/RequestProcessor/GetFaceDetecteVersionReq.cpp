/********************************************************************
* Name      :  GetFaceDetecteVersionReq.cpp                                  
* Describ   :  the definaition of the class
* Author    :  liuxin                                          
* Date      :  2018-06-25                                                
********************************************************************/

#include "GetFaceDetecteVersionReq.h"
#include <stdio.h>
#include "CommunicationAgent.h"
#include "LittleEndianIO.h"
#include "FaceDetector.h"



GetFaceDetecteVersionReq::GetFaceDetecteVersionReq():
	BaseReq( Requestion::GetFaceDetecteVersionReqID )
{
}



int GetFaceDetecteVersionReq::processReq( Requestion *pReq )
{
	UINT8 sendData[UART_GET_FACE_VERSION_SEND_LEN] ={0};

	short int retCode = 0;

	LittleEndianIO::writeInt16((char *)sendData,UART_GET_FACE_VERSION_COMMAND);//Command Code
	sendData[UART_GET_FACE_VERSION_FLAG_LOCATION] = UART_GET_FACE_VERSION_FLAG_SEND_BIT;	//Flags
	sendData[UART_GET_FACE_VERSION_SEQUENCE_NUMBER_LOCATION] = 0; 			//Sequence Number

	 //test
	/*UINT8 *p = sendData;
	for(int i = 0 ; i < UART_GET_FACE_VERSION_SEND_LEN; i++)
	{
		printf("%x ", *p);
		p++;
	}
	printf("\n");
	printf("sendMsg GetFaceDetecteVersionReq ok\n");
	*/
	if(SET_FPGA_TO_CHANGE_BARUDRATE == FaceDetector::getInstance()->getFlagRecoverBarudrate())
	{
		//printf("change to 2");
		FaceDetector::getInstance()->setflagRecoverBarudrate(SEND_GET_SOFET_VERSION_AFTER_CHANGE_FPGA);
	}
	
	return CommunicationAgent::getInstance()->sendMsg(0,UART_GET_FACE_VERSION_COMMAND,
								 0, NULL, 0, CommunicationAgent::SendToOuter);
}

GetFaceDetecteVersionReq::~GetFaceDetecteVersionReq()
{
	
}

