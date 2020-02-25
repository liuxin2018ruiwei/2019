/********************************************************************
* Name      :  ST_9_addFrame.cpp                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  Liu Xin                                          
* Date      :  2018-8-22                                                
********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "DetectFaceIF.h"
#include "ST_9_addFrame.h"
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


int ST_9_addFrame::init_initDetectFaceIF()
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


int ST_9_addFrame:: init_pCallBackInfo()
{
	callbackInfo.RegisterReplyFunc = usrCallbackReplyRegisterClient1;
	callbackInfo.CancelRelpyFunc = usrCallbackReplyCancalClient1;
	callbackInfo.NotifyFaceFunc = usrCallbackNotifyFaceInfo1;
	callbackInfo.NotifyClientFunc = usrCallbackNotifyServiceInfo1;
	void *TFHandle = (void*)1;
	registerCallback( m_TFHandle, &callbackInfo );
	return 0;
}



ST_9_addFrame::ST_9_addFrame(unsigned int nCaseId ): Driver( nCaseId )
{

}

int ST_9_addFrame::init()
{
	init_initDetectFaceIF();
	//printf("init_initDetectFaceIF = ok \n!");
	init_pCallBackInfo();


	int ret32 = startSDK( m_TFHandle );
	if( ret32  < 0)
	{
		printf("startSDK ERR  \n \n");
	}
	
	return 0;
}


int ST_9_addFrame::run()
{
	if(NULL == m_TFHandle)
	{
		m_TFHandle = (void *)1;
	}

	int ret32 = 0;
	UINT8 TestPic[1020*1920]= {0};
	VersionInfo testVersionInfo;

	printf("ST_9_addFrame_1 \n");
	UINT8 *pImage = TestPic;
	ret32 = addFrame(NULL, NULL);
	if(ret32 < 0)
	{
		printf("ST_9_addFrame_1  ok \n \n");
	}else{
		printf("ST_9_addFrame_1 ERR  \n\n");
	}

	printf("ST_9_addFrame_2 \n");
	ret32 = addFrame(m_TFHandle, NULL);
	if(ret32 < 0)
	{
		printf("ST_9_addFrame_2  ok \n \n");
	}else{
		printf("ST_9_addFrame_2 ERR  \n \n");
	}

	printf("ST_9_addFrame_3 \n");
	ret32 = addFrame(NULL, pImage );
	if(ret32 < 0)
	{
		printf("ST_9_addFrame_3  ok \n \n");
	}else{
		printf("ST_9_addFrame_3 ERR  \n\n");
	}

	printf("ST_9_addFrame_4 \n");
	ret32 = addFrame(m_TFHandle, pImage );
	ret32 = addFrame(m_TFHandle, pImage );
	if(ret32 < 0)
	{
		printf("ST_9_addFrame_4  ok \n \n");
	}else{
		printf("ST_9_addFrame_4 ERR  \n\n");
	}

	printf("ST_9_addFrame_5 \n");
	memset(pImage,1,1020*1920);
	ret32 = addFrame(m_TFHandle, pImage );
	if(0 == ret32)
	{
		printf("ST_9_addFrame_5  ok \n \n");
	}else{
		printf("ST_9_addFrame_5 ERR  \n\n");
	}


	printf("-------test addFrame ok \n----- \n");

	
	return 0;
}


int ST_9_addFrame::uninit()
{
	Driver::uninit();
}

ST_9_addFrame::~ST_9_addFrame()
{
	
}



void ST_9_addFrame::usrCallbackReplyCancalClient1(UINT64 *pReqID,
											  UINT8 *pRet,
											  UINT8 *pReplyData,
											  UINT32 *pDataLen )
{
	printf("---the requestion(%lld) CANCAL CLIENT replied: ret=%d, datalen=%d",
		   *pReqID, *pRet, *pDataLen );
	
}


void ST_9_addFrame::usrCallbackReplyRegisterClient1(UINT64 *pReqID,
												UINT8 *pRet,
												UINT8 *pReplyData,
												UINT32 *pDataLen )
{
	printf("---the requestion(%lld) REGISTER CLIENT replied: ret=%d, datalen=%d",
		   *pReqID, *pRet, *pDataLen );
  
}

void ST_9_addFrame::usrCallbackNotifyServiceInfo1( RWNotifyType *pNotifyType,
											   UINT8 *pExtData,
											   UINT32 *pDataLen)
{
	//printf("---the notify CLIENT INFO type=%d, datalen=%d", *pNotifyType, *pDataLen );
}
void ST_9_addFrame::usrCallbackNotifyFaceInfo1( RWNotifyType *pNotifyType,
											UINT8 *pExtData,
											UINT32 *pDataLen )
{
	printf("---the notify IMG INFO type=%d, datalen=%d",
		   *pNotifyType, *pDataLen );
	
}

	 
	

