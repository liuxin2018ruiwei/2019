/********************************************************************
* Name      :  ST_10_getDetectConfig.cpp                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  Liu Xin                                          
* Date      :  2018-8-22                                                
********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "DetectFaceIF.h"
#include "ST_10_getDetectConfig.h"
#include <unistd.h>
#include <string>
#include <cstring>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <memory.h>

#include <unistd.h>




// to_string example
#include <iostream>   // std::cout
#include <string>     // std::string, std::to_string

using namespace std;


int ST_10_getDetectConfig::init_initDetectFaceIF()
{
	userInfo.EncryptType = 1;
	char userName[UserNameLen]="testUser";
	memcpy( (void*)userInfo.UserName, userName, UserNameLen );
	
	char usrPWD[UserPWDLen]="password";
	memcpy( (void*)userInfo.UserPWD, usrPWD, UserPWDLen );
	
	// S2: set the interface offered to login
	
	ifInfo.IPType = 0;
	ifInfo.IPArray[0] = 192;
	ifInfo.IPArray[1] = 168;
	ifInfo.IPArray[2] = 1;
	ifInfo.IPArray[3] = 2;
	ifInfo.CtrlPort = 80;
	ifInfo.DataPort = 81;
	
	
	char devPath[100] = "/dev/ttyS0";
	serialInfo.DevPathPtr = (UINT8*)devPath;
	serialInfo.PathLen = 100;
	serialInfo.Baudrate = 115200;
	
	
	imgInfo.Width = 1080;
	imgInfo.Height = 1920;
	imgInfo.Step = 1080;
	imgInfo.ImgType = 2;
	imgInfo.ImgPtr = &(imgInfo.ImgLen);
	imgInfo.ImgLen = imgInfo.Width*imgInfo.Height;
	
	memset( &cfgInfo, 0, sizeof( DetectConfigInfo ) );
	cfgInfo.DetectType = 1;
	cfgInfo.TransferType = 0x01;
	cfgInfo.BuffCount = 100;
	cfgInfo.CameraID = 1;
	cfgInfo.BuffSize = 100000;
	cfgInfo.SrcUserInfo = &userInfo;
	cfgInfo.SrcIFInfo = &ifInfo;
	cfgInfo.SerialPortInfoPtr = &serialInfo;
	cfgInfo.FrameInfo = &imgInfo;
	

	int sg_baudrate = 115200;
	cfgInfo.SerialPortInfoPtr->Baudrate = sg_baudrate;
	const char *uart_dev = "/dev/ttyAMA2";
	cfgInfo.SerialPortInfoPtr->DevPathPtr = (UINT8 *)uart_dev;
	cfgInfo.SerialPortInfoPtr->PathLen = sizeof("/dev/ttyAMA2");

	/*
	printf("Driver -->uart_dev = %s, Baudrate = %d,DevPathPtr = %s,len = %d\n",
										uart_dev,
											cfgInfo.SerialPortInfoPtr->Baudrate ,
												cfgInfo.SerialPortInfoPtr->DevPathPtr,
													cfgInfo.SerialPortInfoPtr->PathLen);

	*/
	
	//void * TFHandle = (void*)1; 
	int ret32 = initDetectFaceIF( &m_TFHandle, &cfgInfo);
	//printf("ret32 = %d\n",ret32);
	return 0;

}


int ST_10_getDetectConfig:: init_pCallBackInfo()
{
	callbackInfo.RegisterReplyFunc = usrCallbackReplyRegisterClient1;
	callbackInfo.CancelRelpyFunc = usrCallbackReplyCancalClient1;
	callbackInfo.NotifyFaceFunc = usrCallbackNotifyFaceInfo1;
	callbackInfo.NotifyClientFunc = usrCallbackNotifyServiceInfo1;
	void *TFHandle = (void*)1;
	registerCallback( m_TFHandle, &callbackInfo );
	return 0;
}



ST_10_getDetectConfig::ST_10_getDetectConfig(unsigned int nCaseId ): Driver( nCaseId )
{

}

int ST_10_getDetectConfig::init()
{
	init_initDetectFaceIF();
	//printf("init_initDetectFaceIF = ok!");
	init_pCallBackInfo();


	int ret32 = startSDK( m_TFHandle );
	if( ret32  < 0)
	{
		printf("startSDK  ERR \n");
	}
	
	return 0;
}


int ST_10_getDetectConfig::run()
{

	if(NULL == m_TFHandle)
	{
		m_TFHandle = (void *)1;
	}

	int ret32 = 0;
	
	DetectCfg pCfg;
	DetectCfg setCfg;
	setCfg.ConfigMask = 0x7F;
	setCfg.BackgroundCtrl = 2;
	setCfg.DetectHeartbeat= 2;
	setCfg.DetectThreshold= 2;
	setCfg.DetectTime = 2;
	setCfg.DetectType = 2;
	setCfg.FaceCtrl = 2;
	setCfg.FaceSize = 2;
	DetectCfg getCfg;

	printf("ST_10_getDetectConfig_1 \n");
	ret32 = getDetectConfig(NULL, NULL);
	if(ret32 < 0)
	{
		printf("ST_10_getDetectConfig_1  ok\n\n");
	}else{
		printf("ST_10_getDetectConfig_1  ERR\n\n");
	}

	printf("ST_10_getDetectConfig_2 \n");
	ret32 = getDetectConfig(NULL, &pCfg);
	if(ret32 < 0)
	{
		printf("ST_10_getDetectConfig_2  ok\n\n");
	}else{
		printf("ST_10_getDetectConfig_2  ERR\n\n");
	}


	printf("ST_10_getDetectConfig_3 \n");
	setCfg.ConfigMask = 0x12;
	setCfg.FaceCtrl = 0x12;
	setCfg.DetectHeartbeat = 0x02;
	setDetectConfig(m_TFHandle, &setCfg);
	getCfg.ConfigMask = 0x12;
	ret32 = getDetectConfig(m_TFHandle, &getCfg);
	printf("getCfg.FaceCtrl = %d,getCfg.DetectHeartbeat = %d\n",getCfg.FaceCtrl,getCfg.DetectHeartbeat);
	if( (ret32 == 0)&&(0x12 == getCfg.FaceCtrl)&&(2 == getCfg.DetectHeartbeat) )
	{
		printf("ST_10_getDetectConfig_3  ok\n\n");
	}else{
		printf("ST_10_getDetectConfig_3  ERR\n\n");
	}

	printf("ST_10_getDetectConfig_4 \n");
	setCfg.ConfigMask = 0x7F;
	setCfg.BackgroundCtrl = 1;
	setCfg.DetectHeartbeat= 1;
	setCfg.DetectThreshold= 1;
	setCfg.DetectTime = 1;
	setCfg.DetectType = 1;
	setCfg.FaceCtrl = 0x11;
	setCfg.FaceSize = 1;
	setDetectConfig(m_TFHandle, &setCfg);
	getCfg.ConfigMask = 0x7F;
	ret32 = getDetectConfig(m_TFHandle, &getCfg);
	printf("getCfg.DetectThreshold = %d,getCfg.DetectTime = %d\n",getCfg.DetectThreshold,getCfg.DetectTime);
	if( (ret32 == 0)&&(1 == getCfg.BackgroundCtrl)&&(1 == getCfg.DetectHeartbeat)
		            &&(1 == getCfg.DetectThreshold)&&(1 == getCfg.DetectTime)
		            &&(1 == getCfg.DetectType)&&(0x11 == getCfg.FaceCtrl) &&(1 == getCfg.FaceSize))
	{
		printf("ST_10_getDetectConfig_4  ok\n\n");
	}else{
		printf("ST_10_getDetectConfig_4  ERR\n\n");
	}

	

	printf("-------test getDetectConfig ok -----\n");

	return 0;
}


int ST_10_getDetectConfig::uninit()
{
	Driver::uninit();
}

ST_10_getDetectConfig::~ST_10_getDetectConfig()
{
	
}



void ST_10_getDetectConfig::usrCallbackReplyCancalClient1(UINT64 *pReqID,
											  UINT8 *pRet,
											  UINT8 *pReplyData,
											  UINT32 *pDataLen )
{
	printf("---the requestion(%lld) CANCAL CLIENT replied: ret=%d, datalen=%d",
		   *pReqID, *pRet, *pDataLen );
	
}


void ST_10_getDetectConfig::usrCallbackReplyRegisterClient1(UINT64 *pReqID,
												UINT8 *pRet,
												UINT8 *pReplyData,
												UINT32 *pDataLen )
{
	printf("---the requestion(%lld) REGISTER CLIENT replied: ret=%d, datalen=%d",
		   *pReqID, *pRet, *pDataLen );
  
}

void ST_10_getDetectConfig::usrCallbackNotifyServiceInfo1( RWNotifyType *pNotifyType,
											   UINT8 *pExtData,
											   UINT32 *pDataLen)
{
	//printf("---the notify CLIENT INFO type=%d, datalen=%d", *pNotifyType, *pDataLen );
}
void ST_10_getDetectConfig::usrCallbackNotifyFaceInfo1( RWNotifyType *pNotifyType,
											UINT8 *pExtData,
											UINT32 *pDataLen )
{
	printf("---the notify IMG INFO type=%d, datalen=%d",
		   *pNotifyType, *pDataLen );
	
}

	 
	

