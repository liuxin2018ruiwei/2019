/********************************************************************
* Name      :  SoftUpgradeReq.cpp                                  
* Describ   :  the definaition of the class
* Author    :  liuxin                                          
* Date      :  2018-06-28                                                
********************************************************************/

#include "SoftUpgradeReq.h"
#include <stdio.h>

#include "CommunicationAgent.h"
#include "LittleEndianIO.h"
#include "UpgradeRequestion.h"
#include<cstring>
#include "ProcessReq.h"

typedef unsigned char UINT8;
typedef unsigned short int UINT16;


//UINT8 SoftUpgradeReq::m_sequenceNumber = 0;
SoftUpgradeReq::SoftUpgradeReq():
	BaseReq( Requestion::SoftUpgrade )
{
	
}
#define  STREAM (1)
#define  ZMODEM (2)

int SoftUpgradeReq::processReq( Requestion *pReq )
{
	//printf("processReq !!\n");
	if(NULL == pReq)
	{
		return -1;	
	}
	int ret = -1;
	UpgradeRequestion *req = (UpgradeRequestion *)pReq;
	UpgradeDataInfo  & pdata = req->getUpgradeDataInfo();

	UINT8 sendData[UART_SOFT_UPGRADE_SEND_LEN] ={0};
	UINT8 * pfileName = pdata.FileNamPtr;
	UINT8 nameLen =   pdata.NameLen;
	UINT32 fileSize = pdata.FileSize;
	char* pSendFile = (char *)&(sendData[UART_SOFT_UPGRADE_FILE_NAME_LOCATION]);
	short int retCode = 0;

	UINT16 commandType = 0x00;
	UpgradeType mUpgradeType = (UpgradeType)req->getUpgradeDataInfo().UpgradeType;
	if(APP_CFG_TYPE ==  mUpgradeType)
	{
		commandType = UART_SOFT_UPGRADE_APP_CFG_COMMAND;
	}
	else if(APP_PKG_TYPE == mUpgradeType)
	{
		commandType = UART_SOFT_UPGRADE_APP_PKG_COMMAND;
	}
	else if (CAMERA_QSPI_IMAGE_TYPE == mUpgradeType)
	{
		commandType = UART_SOFT_UPGRADE_CAMREA_QSPI_IMAGE_COMMAND;
	}

	LittleEndianIO::writeInt16((char *)sendData,commandType);	//Command Code
	sendData[UART_SOFT_UPGRADE_FLAG_LOCATION] = UART_SOFT_UPGRADE_FLAG_SEND_BIT;	//Flags
	sendData[UART_SOFT_UPGRADE_SEQUENCE_NUMBER_LOCATION] = 0; 			//Sequence Number
	memcpy(pSendFile,pfileName,nameLen);//File Name
	/*for(int i =0 ; (i < nameLen)&&(i < UART_FILE_NAME_LEN); i++)
	{
		sendData[UART_SOFT_UPGRADE_FILE_NAME_LOCATION +i] = *pfileName;
		pfileName++;
	}*/
	memcpy((char*)&(sendData[UART_MID5_LOCATION]),(char *)&(pdata.MD5),UART_MID5_CHECK_SUM_LEN);//MD5 Checksum
	sendData[UART_TRANSFER_METHOD_LOCATION] = ZMODEM;	//Transfer Method
	LittleEndianIO::writeInt32((char *)&(sendData[UART_FILE_SIZE_LOCATION]), fileSize); //FILE SIZE	

	//test
	/*
	UINT8 *p = (UINT8 *)sendData;
	for(int i = 0; i < UART_SOFT_UPGRADE_SEND_LEN; i++)
	{
		printf("%x ",*p);
		p++;
	}
	printf("\n");
	*/
	
	return CommunicationAgent::getInstance()->sendMsg(0,commandType,
								 0, (char *)sendData + UART_SEND_BEGIN_OFFSET, 
								 UART_SOFT_UPGRADE_SEND_LEN - UART_SEND_BEGIN_OFFSET, 
								 CommunicationAgent::SendToOuter);
	return ret;
}

SoftUpgradeReq::~SoftUpgradeReq()
{
	
}

