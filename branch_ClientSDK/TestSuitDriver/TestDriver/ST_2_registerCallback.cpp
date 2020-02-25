/********************************************************************
* Name      :  ST_2_registerCallback.cpp                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  Liu Xin                                          
* Date      :  2018-8-22                                                
********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "DetectFaceIF.h"
#include "ST_2_registerCallback.h"
#include <unistd.h>
#include <string>
#include <cstring>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>




// to_string example
#include <iostream>   // std::cout
#include <string>     // std::string, std::to_string

using namespace std;




ST_2_registerCallback::ST_2_registerCallback(unsigned int nCaseId ): Driver( nCaseId )
{
	
}

int ST_2_registerCallback::init_initDetectFaceIF()
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

	
	/*printf("Driver -->uart_dev = %s, Baudrate = %d,DevPathPtr = %s,len = %d\n",
										uart_dev,
											cfgInfo.SerialPortInfoPtr->Baudrate ,
												cfgInfo.SerialPortInfoPtr->DevPathPtr,
													cfgInfo.SerialPortInfoPtr->PathLen);

	*/
	
	//printf("pCfgInfo = %x\n",*pCfgInfo);
	void * TFHandle = (void*)1; 
	initDetectFaceIF( &TFHandle, &cfgInfo);
	return 0;

}


int ST_2_registerCallback::init()
{
	//Driver::init();
	init_initDetectFaceIF();
	return 0;
}

int ST_2_registerCallback:: init_pCallBackInfo(CallBackInfo **pCallBackInfo)
{
	callbackInfo.RegisterReplyFunc = usrCallbackReplyRegisterClient1;
	callbackInfo.CancelRelpyFunc = usrCallbackReplyCancalClient1;
	callbackInfo.NotifyFaceFunc = usrCallbackNotifyFaceInfo1;
	callbackInfo.NotifyClientFunc = usrCallbackNotifyServiceInfo1;
	*pCallBackInfo  = &callbackInfo;
	return 0;
}


int ST_2_registerCallback::run()
{
	int ret32 = 0;
	CallBackInfo *pCallBackInfo;

	printf("ST_2_registerCallback_1 \n");
	void * TFHandle ;
	TFHandle = NULL; 
	pCallBackInfo = NULL;
	ret32 = registerCallback( TFHandle, pCallBackInfo );
	if( ret32 < 0 )
	{
		printf("ST_2_registerCallback_1 OK \n \n");
	}else{
		printf("ST_2_registerCallback_1 ERR \n\n");
	}


	printf("ST_2_registerCallback_2 \n");
	TFHandle = (void *)1;
	pCallBackInfo = NULL;
	ret32 = registerCallback( TFHandle, pCallBackInfo );
	if( ret32 < 0 )
	{
		printf("ST_2_registerCallback_2 OK \n \n");
	}else{
		printf("ST_2_registerCallback_2 ERR \n\n");
	}

	printf("ST_2_registerCallback_3 \n");
	TFHandle = (void *)1; 
	init_pCallBackInfo(&pCallBackInfo);
	pCallBackInfo->RegisterReplyFunc =NULL;
	ret32 = registerCallback( TFHandle, pCallBackInfo );
	if( ret32 < 0 )
	{
		printf("ST_2_registerCallback_3 OK \n \n");
	}else{
		printf("ST_2_registerCallback_3 ERR \n\n");
	}

	printf("ST_2_registerCallback_4 \n");
	TFHandle = (void *)1; 
	init_pCallBackInfo(&pCallBackInfo);
	pCallBackInfo->CancelRelpyFunc  = NULL;
	ret32 = registerCallback( TFHandle,pCallBackInfo );
	if( ret32 < 0 )
	{
		printf("ST_2_registerCallback_4 OK \n \n");
	}else{
		printf("ST_2_registerCallback_4 ERR \n\n");
	}

	printf("ST_2_registerCallback_5 \n");
	TFHandle = (void *)1;  
	init_pCallBackInfo(&pCallBackInfo);
	pCallBackInfo->NotifyFaceFunc  = NULL;
	ret32 = registerCallback( TFHandle, pCallBackInfo );
	if( ret32 < 0 )
	{
		printf("ST_2_registerCallback_5 OK \n \n");
	}else{
		printf("ST_2_registerCallback_5 ERR \n\n");
	}

	printf("ST_2_registerCallback_6 \n");
	TFHandle = (void *)1;
	init_pCallBackInfo(&pCallBackInfo);
	pCallBackInfo->NotifyClientFunc = NULL;
	ret32 = registerCallback( TFHandle, pCallBackInfo );
	if( ret32 < 0 )
	{
		printf("ST_2_registerCallback_6 OK \n \n");
	}else{
		printf("ST_2_registerCallback_6 ERR \n\n");
	}

	printf("ST_2_registerCallback_7 \n");
	TFHandle = (void *)1; 
	init_pCallBackInfo(&pCallBackInfo);
	ret32 = registerCallback( TFHandle, pCallBackInfo );
	if( 0 == ret32 )
	{
		printf("ST_2_registerCallback_7 OK \n \n");
	}else{
		printf("ST_2_registerCallback_7 ERR \n\n");
	}

	printf("run \n");
	return 0;
}


int ST_2_registerCallback::uninit()
{
	Driver::uninit();
}

ST_2_registerCallback::~ST_2_registerCallback()
{
	
}



void ST_2_registerCallback::usrCallbackReplyCancalClient1(UINT64 *pReqID,
											  UINT8 *pRet,
											  UINT8 *pReplyData,
											  UINT32 *pDataLen )
{
	printf("---the requestion(%lld) CANCAL CLIENT replied: ret=%d, datalen=%d",
		   *pReqID, *pRet, *pDataLen );
	
}


void ST_2_registerCallback::usrCallbackReplyRegisterClient1(UINT64 *pReqID,
												UINT8 *pRet,
												UINT8 *pReplyData,
												UINT32 *pDataLen )
{
	printf("---the requestion(%lld) REGISTER CLIENT replied: ret=%d, datalen=%d",
		   *pReqID, *pRet, *pDataLen );
  
}

void ST_2_registerCallback::usrCallbackNotifyServiceInfo1( RWNotifyType *pNotifyType,
											   UINT8 *pExtData,
											   UINT32 *pDataLen)
{
	//printf("---the notify CLIENT INFO type=%d, datalen=%d", *pNotifyType, *pDataLen );
}
void ST_2_registerCallback::usrCallbackNotifyFaceInfo1( RWNotifyType *pNotifyType,
											UINT8 *pExtData,
											UINT32 *pDataLen )
{
	printf("---the notify IMG INFO type=%d, datalen=%d",
		   *pNotifyType, *pDataLen );
	
}

	 
	

