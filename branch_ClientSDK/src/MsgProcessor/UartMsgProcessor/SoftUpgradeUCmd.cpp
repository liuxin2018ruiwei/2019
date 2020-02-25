/********************************************************************
* Name      :  SoftUpgradeUCmd.cpp                                  
* Describ   :  get the heart beat values
* Author    :  liuxin                                        
* Date      :  2018-06-26                                                 
********************************************************************/
#include <stdio.h>
#include "SoftUpgradeUCmd.h"
#include "UartCommandType.h"
#include "LittleEndianIO.h"
#include "ProcessReq.h"
#include "Requestion.h"
#include "HeartBeatInfo.h"
#include "SoftUpgradeReq.h"
#include "SoftUpgradeAppPkgCmd.h"
#include "DetectFaceIFCommon.h"
#include "CommunicationAgent.h"
#include "ZmodeUpgrade.h"
#include "FaceDetector.h"
#include "ConfigManage.h"
#include <unistd.h>





SoftUpgradeUCmd::SoftUpgradeUCmd():UartCommand( UartCommandCode::QUERY_CAMERA_QSPI_IMAGE_UPGRADE ) 
{
	
}

int SoftUpgradeUCmd::processMsg(Message * pMsg)
{
	if(NULL == pMsg)
	{
		return -1;
	}
	
	short int retCode = 0;
	UINT8  flags = pMsg->getFlags(); //Flags
	UINT8* p = (UINT8 *)(pMsg->getData());

	CallbackReplyFunc pReplyFunc = ProcessReq::getInstance()->getUpgradeReplyFunC();
	RWNotifyType myRWNotifyType = NotifyState;
	UINT8 pRet = 0;
	UINT32 pDataLen = 1;
	UINT64 reqID = Requestion::SoftUpgrade;

	//err
	if(1 == (flags & 0x01) ) // bit[0] == 1 err
	{
		RWErrorInfo err;
		err.DataLen =1;
		err.ErrorCode = LittleEndianIO::readInt32((const char*)p);
		UINT8 *pReplyData = (UINT8 *)&err;
		pRet = 1; //err
		pDataLen = sizeof(RWErrorInfo);
		(*pReplyFunc)(&reqID,  &pRet, pReplyData, &pDataLen);
		HeartBeatInfo::getInstance()->setUpState(UPGREAD_STOP);
		return -1;
	}
		
	else if( flags == UART_SOFT_UPGRADE_REC_CONTINUE_BIT)
	{
		HeartBeatInfo::getInstance()->setUpState(UPGREAD_START);
		
		UpgradeReqType pReplyData =UpgradeReqType ::  ReqStartUpgrade;//UpgradeReqType :ReqStartUpgrade;
		UINT8 pRet = 0;
		UINT32 pDataLen = 1;
		(*pReplyFunc)(&reqID,  &pRet,  (UINT8 *)(&pReplyData), &pDataLen);
		
		char * pathAndFileName = FaceDetector::getInstance()->getUpdateFileName();
		int ret32 = ConfigManage::getInstance()->updateApp(pathAndFileName, CAMERA_QSPI_IMAGE_TYPE);
		if(0 != ret32)
		{
			return -1;
		}
		return 0;
	}
	//active uart send
	else if( flags == UART_SOFT_UPGRADE_REC_BIT  )
	{
		sleep(2);//加上延时以防万一
		//开心跳
		HeartBeatInfo::getInstance()->setUpState(UPGREAD_STOP);
		return CommunicationAgent::getInstance()->sendMsg(UART_Protocol,UART_ACTIVE_APP_CAMERA_QPSI_IAMGE_COMMAND,
									 retCode, NULL, 0, CommunicationAgent::SendToOuter);
	}
	return -1;
}
SoftUpgradeUCmd::~SoftUpgradeUCmd()
{
}
