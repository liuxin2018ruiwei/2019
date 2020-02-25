/********************************************************************
* Name      :  IT_02_detectFace_05.cpp                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  Liu Xin                                          
* Date      :  2018-8-31                                                
********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "DetectFaceIF.h"
#include "IT_02_detectFace_05.h"
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


int IT_02_detectFace_05::init_initDetectFaceIF()
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


int IT_02_detectFace_05:: init_pCallBackInfo()
{
	callbackInfo.RegisterReplyFunc = usrCallbackReplyRegisterClient2;
	callbackInfo.CancelRelpyFunc = usrCallbackReplyCancalClient2;
	callbackInfo.NotifyFaceFunc = usrCallbackNotifyFaceInfo2;
	printf("callbackInfo.NotifyFaceFunc : %x\n",callbackInfo.NotifyFaceFunc);
	callbackInfo.NotifyClientFunc = usrCallbackNotifyServiceInfo2;
	registerCallback( m_TFHandle, &callbackInfo );
	return 0;
}



IT_02_detectFace_05::IT_02_detectFace_05(unsigned int nCaseId ): Driver( nCaseId )
{

}

int IT_02_detectFace_05::init()
{
	init_pCallBackInfo();
	init_initDetectFaceIF();
	//printf("init_initDetectFaceIF = ok!");
	


	int ret32 = startSDK( m_TFHandle );
	if( ret32  < 0)
	{
		printf("startSDK  ERR \n");
	}
	
	return 0;
}


int IT_02_detectFace_05::run()
{	
	int ret32 = 0;
	DetectCfg cfgData;
	cfgData.ConfigMask = 0x03;
	cfgData.DetectHeartbeat = 0;
	cfgData.DetectType = 0;
	printf("\n IT_02_detectFace_05  close detect face \n");
	ret32 = setDetectConfig(m_TFHandle, &cfgData);



	cfgData.DetectType = 1;
	printf("\n IT_02_detectFace_05  open detect face \n");
	ret32 = setDetectConfig(m_TFHandle, &cfgData);


	

while(1);
	
	return 0;
}



int IT_02_detectFace_05::uninit()
{
	Driver::uninit();
}

IT_02_detectFace_05::~IT_02_detectFace_05()
{
	
}



void IT_02_detectFace_05::usrCallbackReplyCancalClient2(UINT64 *pReqID,
											  UINT8 *pRet,
											  UINT8 *pReplyData,
											  UINT32 *pDataLen )
{
	printf("---the requestion(%lld) CANCAL CLIENT replied: ret=%d, datalen=%d",
		   *pReqID, *pRet, *pDataLen );
	
}


void IT_02_detectFace_05::usrCallbackReplyRegisterClient2(UINT64 *pReqID,
												UINT8 *pRet,
												UINT8 *pReplyData,
												UINT32 *pDataLen )
{
	printf("---the requestion(%lld) REGISTER CLIENT replied: ret=%d, datalen=%d",
		   *pReqID, *pRet, *pDataLen );
  
}

void IT_02_detectFace_05::usrCallbackNotifyServiceInfo2( RWNotifyType *pNotifyType,
											   UINT8 *pExtData,
											   UINT32 *pDataLen)
{
	printf("---the notify CLIENT INFO type=%d, datalen=%d", *pNotifyType, *pDataLen );
}
void IT_02_detectFace_05::usrCallbackNotifyFaceInfo2( RWNotifyType *pNotifyType,
											UINT8 *pExtData,
											UINT32 *pDataLen )
{
	printf("---[LX ]the notify IMG INFO type=%d, datalen=%d", *pNotifyType, *pDataLen );


	printf("---->--->-->%x\n",pExtData);
	RWFaceInfo * mRWFaceInfo = (RWFaceInfo *)pExtData ;
	int len = (*pDataLen)/sizeof(RWFaceInfo);
	printf("len = %d\n",len);

	for(int i = 0; i < len; i++)
	{
		printf("mRWFaceInfo[%d] = %x\n",i,mRWFaceInfo[i].TrackNo);
	}
	
}

	 
	
