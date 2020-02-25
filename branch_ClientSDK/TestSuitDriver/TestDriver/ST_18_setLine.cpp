/********************************************************************
* Name      :  ST_18_setLine.cpp                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  Liu Xin                                          
* Date      :  2018-8-22                                                
********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "DetectFaceIF.h"
#include "ST_18_setLine.h"
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


int ST_18_setLine::init_initDetectFaceIF()
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


int ST_18_setLine:: init_pCallBackInfo()
{
	callbackInfo.RegisterReplyFunc = usrCallbackReplyRegisterClient1;
	callbackInfo.CancelRelpyFunc = usrCallbackReplyCancalClient1;
	callbackInfo.NotifyFaceFunc = usrCallbackNotifyFaceInfo1;
	callbackInfo.NotifyClientFunc = usrCallbackNotifyServiceInfo1;
	registerCallback( m_TFHandle, &callbackInfo );
	return 0;
}



ST_18_setLine::ST_18_setLine(unsigned int nCaseId ): Driver( nCaseId )
{

}

int ST_18_setLine::init()
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


void testCallbackSetLine( RWNotifyType *pNotifyType, UINT8 *pExtData, UINT32 *pDataLen )
{

	typedef struct _strCounterInfo
	{
		unsigned int ComeIn;
		unsigned int GetOut;
	}CounterInfo;
	CounterInfo * info = (CounterInfo*)pExtData;
	printf("use  comein = %d, getout=%d\n",info->ComeIn,info->GetOut);
	printf("for my fata! ");
}


int ST_18_setLine::run()
{
	CallbackNotifyFunc pFunc = testCallbackSetLine;
	CallbackNotifyFunc pFunctestNULL= NULL;

	Line line1;
	line1.Head.X = 12;
	line1.Head.Y = 34;
	line1.End.X = 56;
	line1.End.Y = 720;

	int width = 1080;
	int heigth = 1920;

	Line line2;
	line2.Head.X = 18;
	line2.Head.Y = 120;
	line2.End.X = width -1;
	line2.End.Y = 620;

	Line line3;
	line3.Head.X = 20;
	line3.Head.Y = 20;
	line3.End.X = 56;
	line3.End.Y = 220;

	Line line4;
	line4.Head.X = 4;
	line4.Head.Y =320;
	line4.End.X = width;
	line4.End.Y = 320;
	
	Line line5;
	line5.Head.X = 0;
	line5.Head.Y = 420;
	line5.End.X = width;
	line5.End.Y = 420;
	

	
	printf("ST_18_setLine_1 \n");
	int ret32 = setLine( NULL, NULL,NULL);
	if(ret32 < 0)
	{
		printf("ST_18_setLine_1  ok\n\n");
	}else{
		printf("ST_18_setLine_1  ERR\n\n");
	}

	printf("ST_18_setLine_2 \n");
	ret32 = setLine( m_TFHandle, NULL,NULL);
	if(ret32 < 0)
	{
		printf("ST_18_setLine_2  ok\n\n");
	}else{
		printf("ST_18_setLine_2  ERR\n\n");
	}


	printf("ST_18_setLine_3 \n");
	ret32 = setLine( NULL, &line1, NULL);
	if(ret32 < 0)
	{
		printf("ST_18_setLine_3  ok\n\n");
	}else{
		printf("ST_18_setLine_3  ERR\n\n");
	}

	printf("ST_18_setLine_4 \n");
	Line lineErr;
	lineErr.Head.X = 0;
	lineErr.Head.Y = 0;
	lineErr.End.X = width;
	lineErr.End.Y = 0;
	ret32 = setLine( m_TFHandle, &lineErr, pFunctestNULL);
	if(ret32 < 0)
	{
		printf("ST_18_setLine_4  ok\n\n");
	}else{
		printf("ST_18_setLine_4  ERR\n\n");
	}

	printf("ST_18_setLine_5 \n");
	//Line lineErr;
	lineErr.Head.X = 0;
	lineErr.Head.Y = heigth;
	lineErr.End.X = width;
	lineErr.End.Y = heigth;
	ret32 = setLine( m_TFHandle, &lineErr, pFunctestNULL);
	if(ret32 < 0)
	{
		printf("ST_18_setLine_5  ok\n\n");
	}else{
		printf("ST_18_setLine_5  ERR\n\n");
	}

	printf("ST_18_setLine_6 \n");
	//Line lineErr;
	lineErr.Head.X = 0;
	lineErr.Head.Y =123;
	lineErr.End.X = 0;
	lineErr.End.Y = 212;
	ret32 = setLine( m_TFHandle, &lineErr, pFunctestNULL);
	if(ret32 == 0)
	{
		printf("ST_18_setLine_6  ok\n\n");
	}else{
		printf("ST_18_setLine_6  ERR\n\n");
	}


	printf("ST_18_setLine_7 \n");
	ret32 = setLine( m_TFHandle, &line1, pFunctestNULL);
	if(ret32 == 0)
	{
		printf("ST_18_setLine_7  ok\n\n");
	}else{
		printf("ST_18_setLine_7  ERR\n\n");
	}

	printf("ST_18_setLine_8 \n");
	ret32 = setLine( m_TFHandle, &line1, pFunc);
	if(ret32 == 0)
	{
		printf("ST_18_setLine_8  ok\n\n");
	}else{
		printf("ST_18_setLine_8  ERR\n\n");
	}

	printf("ST_18_setLine_9 \n");
	ret32 = setLine( m_TFHandle, &line2, pFunc);
	if(ret32 == 0)
	{
		printf("ST_18_setLine_9  ok\n\n");
	}else{
		printf("ST_18_setLine_9  ERR\n\n");
	}
	
	printf("ST_18_setLine_10 \n");
	ret32 = setLine( m_TFHandle, &line3, pFunc);
	ret32 |= setLine( m_TFHandle, &line4, pFunc);
	if(ret32 < 0)
	{
		printf("ST_18_setLine_10  ok\n\n");
	}else{
		printf("ST_18_setLine_10  ERR\n\n");
	}


	printf("-------test ST_18_setLine ok-----\n");
	return 0;
}



int ST_18_setLine::uninit()
{
	Driver::uninit();
}

ST_18_setLine::~ST_18_setLine()
{
	
}



void ST_18_setLine::usrCallbackReplyCancalClient1(UINT64 *pReqID,
											  UINT8 *pRet,
											  UINT8 *pReplyData,
											  UINT32 *pDataLen )
{
	printf("---the requestion(%lld) CANCAL CLIENT replied: ret=%d, datalen=%d",
		   *pReqID, *pRet, *pDataLen );
	
}


void ST_18_setLine::usrCallbackReplyRegisterClient1(UINT64 *pReqID,
												UINT8 *pRet,
												UINT8 *pReplyData,
												UINT32 *pDataLen )
{
	printf("---the requestion(%lld) REGISTER CLIENT replied: ret=%d, datalen=%d",
		   *pReqID, *pRet, *pDataLen );
  
}

void ST_18_setLine::usrCallbackNotifyServiceInfo1( RWNotifyType *pNotifyType,
											   UINT8 *pExtData,
											   UINT32 *pDataLen)
{
	//printf("---the notify CLIENT INFO type=%d, datalen=%d", *pNotifyType, *pDataLen );
}
void ST_18_setLine::usrCallbackNotifyFaceInfo1( RWNotifyType *pNotifyType,
											UINT8 *pExtData,
											UINT32 *pDataLen )
{
	printf("---the notify IMG INFO type=%d, datalen=%d",
		   *pNotifyType, *pDataLen );
	
}

	 
	

