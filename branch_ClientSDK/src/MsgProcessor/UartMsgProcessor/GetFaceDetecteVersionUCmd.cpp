/********************************************************************
* Name      :  GetFaceDetecteVersionUCmd.cpp                                  
* Describ   :  get the heart beat values
* Author    :  liuxin                                        
* Date      :  2018-06-26                                                 
********************************************************************/
#include <stdio.h>
#include "GetFaceDetecteVersionUCmd.h"
#include "GetFaceDetecteVersionReq.h"
#include "LittleEndianIO.h"
#include "UartCommandType.h"

#include "cstring"
#include "ConfigManage.h"
#include "DetectFaceIFCommon.h"
#include "FaceDetector.h"


GetFaceDetecteVersionUCmd::GetFaceDetecteVersionUCmd():UartCommand( UartCommandCode::QUERY_FACE_DETECTE_VERSION ) 
{
	
}

int GetFaceDetecteVersionUCmd::processMsg(Message * pMsg )
{
	if(NULL == pMsg)
	{
		return -1;
	}
	
	UINT16 commandCode= pMsg->getCode();//Command Code
	UINT8  flags = pMsg->getFlags(); //Flags
	UINT8  sequenceNumber = pMsg->getSequenceNumber(); //Sequence Number
	UINT8* p = (UINT8 *)(pMsg->getData());

	//printf("get ok!!processMsg commandCode = %x\n",commandCode);
	//get data
	char packageVersion[PACKAGE_VERSION_LEN]= {0};
	memcpy(packageVersion,p,PACKAGE_VERSION_LEN);

	p = p + PACKAGE_VERSION_LEN;
	char systemVersion[SYSTEME_VERSION_LEN] = {0};
	memcpy(systemVersion,p, SYSTEME_VERSION_LEN);

	if(SEND_GET_SOFET_VERSION_AFTER_CHANGE_FPGA == FaceDetector::getInstance()->getFlagRecoverBarudrate())
	{
		FaceDetector::getInstance()->setflagRecoverBarudrate(GET_DATA_AFTER_SEND_GET_VERION);
		//printf("change to 3!!!\n");
	}
	
	
	if((flags == UART_GET_FACE_VWESION_FLAG_REC_BIT) && ((flags & 0x01) == 0x00) )
	{
		ConfigManage::getInstance()->setModuleSoftwareVersion(packageVersion,PACKAGE_VERSION_LEN);
		ConfigManage::getInstance()->setModuleFirmwareVersion(systemVersion,SYSTEME_VERSION_LEN);
		return 0;
	}
	
	//
	return 0;
}
GetFaceDetecteVersionUCmd::~GetFaceDetecteVersionUCmd()
{
}
