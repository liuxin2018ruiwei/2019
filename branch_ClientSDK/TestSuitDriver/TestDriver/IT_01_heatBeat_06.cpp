/********************************************************************
* Name      :  IT_01_heatBeat_06.cpp                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  Liu Xin                                          
* Date      :  2018-8-31                                                
********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "DetectFaceIF.h"
#include "IT_01_heatBeat_06.h"
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


int IT_01_heatBeat_06::init_initDetectFaceIF()
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


int IT_01_heatBeat_06:: init_pCallBackInfo()
{
	callbackInfo.RegisterReplyFunc = usrCallbackReplyRegisterClient1;
	callbackInfo.CancelRelpyFunc = usrCallbackReplyCancalClient1;
	callbackInfo.NotifyFaceFunc = usrCallbackNotifyFaceInfo1;
	callbackInfo.NotifyClientFunc = usrCallbackNotifyServiceInfo1;
	registerCallback( m_TFHandle, &callbackInfo );
	return 0;
}



IT_01_heatBeat_06::IT_01_heatBeat_06(unsigned int nCaseId ): Driver( nCaseId )
{

}

int IT_01_heatBeat_06::init()
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


int IT_01_heatBeat_06::run()
{	
	int ret32 = 0;
	DetectCfg cfgData;
	cfgData.ConfigMask = 0x02;

	printf("\n\nIT_01_heatBeat_06\n");

		
	cfgData.DetectHeartbeat = 0;
	ret32 = setDetectConfig(m_TFHandle, &cfgData);
	if(ret32 == 0)
	{
		printf("close send 0x0102 = %d \n",cfgData.DetectHeartbeat);
	}

	printf("sleep 10s test connection \n\n");
	sleep(10);

			
	cfgData.DetectHeartbeat = 0;
	ret32 = setDetectConfig(m_TFHandle, &cfgData);
	if(ret32 == 0)
	{
		printf("close send 0x0102 = %d \n",cfgData.DetectHeartbeat);
	}

	printf("sleep 10s test connection \n\n");
	sleep(10);
	
	
	cfgData.DetectHeartbeat = 10;
	ret32 = setDetectConfig(m_TFHandle, &cfgData);
	if(ret32 == 0)
	{
		printf("open send 0x0102 = %d \n",cfgData.DetectHeartbeat);
	}
	sleep(10);
	

	return 0;
}



int IT_01_heatBeat_06::uninit()
{
	Driver::uninit();
}

IT_01_heatBeat_06::~IT_01_heatBeat_06()
{
	
}



void IT_01_heatBeat_06::usrCallbackReplyCancalClient1(UINT64 *pReqID,
											  UINT8 *pRet,
											  UINT8 *pReplyData,
											  UINT32 *pDataLen )
{
	printf("---the requestion(%lld) CANCAL CLIENT replied: ret=%d, datalen=%d",
		   *pReqID, *pRet, *pDataLen );
	
}


void IT_01_heatBeat_06::usrCallbackReplyRegisterClient1(UINT64 *pReqID,
												UINT8 *pRet,
												UINT8 *pReplyData,
												UINT32 *pDataLen )
{
	printf("---the requestion(%lld) REGISTER CLIENT replied: ret=%d, datalen=%d",
		   *pReqID, *pRet, *pDataLen );
  
}

void IT_01_heatBeat_06::usrCallbackNotifyServiceInfo1( RWNotifyType *pNotifyType,
											   UINT8 *pExtData,
											   UINT32 *pDataLen)
{
	printf("---the notify CLIENT INFO type=%d, datalen=%d", *pNotifyType, *pDataLen );
}
void IT_01_heatBeat_06::usrCallbackNotifyFaceInfo1( RWNotifyType *pNotifyType,
											UINT8 *pExtData,
											UINT32 *pDataLen )
{
	printf("---the notify IMG INFO type=%d, datalen=%d",
		   *pNotifyType, *pDataLen );
	
}

	 
	

