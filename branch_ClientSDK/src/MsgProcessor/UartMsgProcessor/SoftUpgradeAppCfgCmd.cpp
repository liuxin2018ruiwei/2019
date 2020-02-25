/********************************************************************
* Name      :  SoftUpgradeAppCfgCmd.cpp                                  
* Describ   :  get the heart beat values
* Author    :  liuxin                                        
* Date      :  2018-06-26                                                 
********************************************************************/
#include <stdio.h>
#include "SoftUpgradeAppCfgCmd.h"
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
#include <unistd.h>

#include "ConfigManage.h"



SoftUpgradeAppCfgCmd::SoftUpgradeAppCfgCmd():UartCommand( UartCommandCode::QUERY_APP_CFG_UPGRADE ) 
{
	 
}

int SoftUpgradeAppCfgCmd::processMsg(Message * pMsg)
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


	//printf("upgrade cfg --> get flags = %x\n",flags);

	//err
	if(  1 == (flags & 0x01) ) // bit[0] == 1 err
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

		printf("updateApp ReqStartUpgrade;\n");
		UpgradeReqType pReplyData = UpgradeReqType :: ReqStartUpgrade ;
		UINT8 pRet = 0;
		UINT32 pDataLen = 1;
		(*pReplyFunc)(&reqID,  &pRet, (UINT8 *)(&pReplyData), &pDataLen);
		
		//升级前关心跳
		HeartBeatInfo::getInstance()->setUpState(UPGREAD_START);
		
		// zmood 升级
		char * pathAndFileName = FaceDetector::getInstance()->getUpdateFileName();
		int ret32 = ConfigManage::getInstance()->updateApp(pathAndFileName, APP_CFG_TYPE);
		if(0 != ret32)
		{
			printf("ERR : updateApp ret = %d\n",ret32);
			return -1;
		}
		//return 0;
	}
	//else if( flags == UART_SOFT_UPGRADE_REC_BIT )
	{
		sleep(2);//要延时2s,测试发现app.cfg 没有发送0x0231的80回复
		//升级完开心跳
		HeartBeatInfo::getInstance()->setUpState(UPGREAD_STOP);
		return CommunicationAgent::getInstance()->sendMsg(UART_Protocol,UART_ACTIVE_APP_CFG_COMMAND,
									 retCode, NULL, 0, CommunicationAgent::SendToOuter);
	}
	return -1;
	
}
SoftUpgradeAppCfgCmd::~SoftUpgradeAppCfgCmd()
{
}
