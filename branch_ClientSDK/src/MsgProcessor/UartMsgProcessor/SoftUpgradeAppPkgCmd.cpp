/********************************************************************
* Name      :  SoftUpgradeAppPkgCmd.cpp                                  
* Describ   : 
* Author    :  liuxin                                        
* Date      :  2018-06-26                                                 
********************************************************************/
#include <stdio.h>
#include "SoftUpgradeAppPkgCmd.h"
#include "UartCommandType.h"
#include "LittleEndianIO.h"
#include "ProcessReq.h"
#include "Requestion.h"
#include "HeartBeatInfo.h"
#include "SoftUpgradeReq.h"
#include "DetectFaceIFCommon.h"
#include "CommunicationAgent.h"
#include "ZmodeUpgrade.h"
#include "FaceDetector.h"

#include "ConfigManage.h"
#include <unistd.h>



SoftUpgradeAppPkgCmd::SoftUpgradeAppPkgCmd():UartCommand( UartCommandCode::QUERY_APP_PKG_UPGRADE ) 
{
	
}

int SoftUpgradeAppPkgCmd::processMsg(Message * pMsg)
{
	if(NULL == pMsg)
	{
		return -1;
	}
	
	UINT8  flags = pMsg->getFlags(); //Flags

	short int retCode = 0;
	UINT8* p = (UINT8 *)(pMsg->getData());


	CallbackReplyFunc pReplyFunc = ProcessReq::getInstance()->getUpgradeReplyFunC();
	RWNotifyType myRWNotifyType = NotifyState;
	UINT8 pRet = 0;
	UINT32 pDataLen = 1;
	UINT64 reqID = Requestion::SoftUpgrade;


	//err
	if( 1 == (flags & 0x01) ) // bit[0] == 1 err
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
		
	else if( flags == UART_SOFT_UPGRADE_REC_CONTINUE_BIT )
	{
		
	//	printf("ReqStartUpgrade\n"); 
		UpgradeReqType pReplyData = UpgradeReqType :: ReqStartUpgrade;
		UINT8 pRet = 0;
		UINT32 pDataLen = 1;
		(*pReplyFunc)(&reqID,  &pRet, (UINT8 *)(&pReplyData), &pDataLen);

		//关心跳
	    HeartBeatInfo::getInstance()->setUpState(UPGREAD_START);
		//升级
		char * pathAndFileName = FaceDetector::getInstance()->getUpdateFileName();
		int ret32 = ConfigManage::getInstance()->updateApp(pathAndFileName, APP_PKG_TYPE);
		if(-1 == ret32)
		{
			return -1;
		}
		return 0;
	}
	else if( flags ==   UART_SOFT_UPGRADE_REC_BIT)
	{
		sleep(2);// 要延时下，不然FPGA有时可能没办法启动
		//开心跳
		HeartBeatInfo::getInstance()->setUpState(UPGREAD_STOP);	
		//发送激活命令
		return CommunicationAgent::getInstance()->sendMsg(UART_Protocol,UART_ACTIVE_APP_PKG_COMMAND,
									 retCode, NULL, 0, CommunicationAgent::SendToOuter);
	}
	return -1;
}
SoftUpgradeAppPkgCmd::~SoftUpgradeAppPkgCmd()
{
}
