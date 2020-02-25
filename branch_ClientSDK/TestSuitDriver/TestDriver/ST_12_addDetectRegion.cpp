/********************************************************************
* Name      :  ST_12_addDetectRegion.cpp                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  Liu Xin                                          
* Date      :  2018-8-22                                                
********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "DetectFaceIF.h"
#include "ST_12_addDetectRegion.h"
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


int ST_12_addDetectRegion::init_initDetectFaceIF()
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


int ST_12_addDetectRegion:: init_pCallBackInfo()
{
	callbackInfo.RegisterReplyFunc = usrCallbackReplyRegisterClient1;
	callbackInfo.CancelRelpyFunc = usrCallbackReplyCancalClient1;
	callbackInfo.NotifyFaceFunc = usrCallbackNotifyFaceInfo1;
	callbackInfo.NotifyClientFunc = usrCallbackNotifyServiceInfo1;
	void *TFHandle = (void*)1;
	registerCallback( m_TFHandle, &callbackInfo );
	return 0;
}



ST_12_addDetectRegion::ST_12_addDetectRegion(unsigned int nCaseId ): Driver( nCaseId )
{

}

int ST_12_addDetectRegion::init()
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


int ST_12_addDetectRegion::run()
{

	if(NULL == m_TFHandle)
	{
		m_TFHandle = (void *)1;
	}

	int ret32 = 0;

	int nCount = 4;	
	Point tempPoint4[4];
	tempPoint4[0].X = 50;	
	tempPoint4[0].Y = 20;	
	tempPoint4[1].X = 100;	
	tempPoint4[1].Y = 20;	
	tempPoint4[2].X = 100;	
	tempPoint4[2].Y = 100;	
	tempPoint4[3].X = 50;	
	tempPoint4[3].Y = 100;	
	//printf("LXTestWBS_02_10_19 nCount = %d",nCount);
	//addDetectRegion( m_TFHandle, (Point *)(&tempPoint1),  nCount );
	
	Point *pPoint;

	printf("ST_12_addDetectRegion_1 \n");
	ret32 = addDetectRegion( NULL, NULL, 0 );
	if(ret32 < 0)
	{
		printf("ST_12_addDetectRegion_1  ok\n\n");
	}else{
		printf("ST_12_addDetectRegion_1  ERR\n\n");
	}

	printf("ST_12_addDetectRegion_2 \n");
	ret32 = addDetectRegion( NULL, (Point *)(&tempPoint4), 0 );
	if(ret32 < 0)
	{
		printf("ST_12_addDetectRegion_2  ok\n\n");
	}else{
		printf("ST_12_addDetectRegion_2  ERR\n\n");
	}

	printf("ST_12_addDetectRegion_3 \n");
	ret32 = addDetectRegion( m_TFHandle, (Point *)(&tempPoint4), 0 );
	if(ret32 < 0)
	{
		printf("ST_12_addDetectRegion_3  ok\n\n");
	}else{
		printf("ST_12_addDetectRegion_3  ERR\n\n");
	}


	printf("ST_12_addDetectRegion_4 \n");
	ret32 = addDetectRegion( m_TFHandle, (Point *)(&tempPoint4), nCount );
	if(ret32 == 0)
	{
		printf("ST_12_addDetectRegion_4  ok\n\n");
	}else{
		printf("ST_12_addDetectRegion_4  ERR\n\n");
	}

	printf("ST_12_addDetectRegion_5 \n");
	nCount = 8;	
	Point tempPoint8[8];
	tempPoint8[0].X = 50;	
	tempPoint8[0].Y = 20;	
	tempPoint8[1].X = 80;	
	tempPoint8[1].Y = 20;	
	tempPoint8[2].X = 80;	
	tempPoint8[2].Y = 30;	
	tempPoint8[3].X = 100;	
	tempPoint8[3].Y = 30;	
	tempPoint8[4].X = 100;	
	tempPoint8[4].Y = 20;	
	tempPoint8[5].X = 130;	
	tempPoint8[5].Y = 20;	
	tempPoint8[6].X = 130;	
	tempPoint8[6].Y = 50;	
	tempPoint8[7].X = 50;	
	tempPoint8[7].Y = 50;
	ret32 = addDetectRegion( m_TFHandle, (Point *)(&tempPoint8),  nCount );
	if(ret32 == 0)
	{
		printf("ST_12_addDetectRegion_5  ok\n\n");
	}
	else
	{
		printf("ST_12_addDetectRegion_5  ERR\n\n");
	}


	printf("ST_12_addDetectRegion_6 \n");
	nCount = 10 ; 
	Point tempPoint10[10];
	tempPoint10[0].X = 50;	
	tempPoint10[0].Y = 20;	
	tempPoint10[1].X = 80;	
	tempPoint10[1].Y = 20;	
	tempPoint10[2].X = 80;	
	tempPoint10[2].Y = 30;	
	tempPoint10[3].X = 100;	
	tempPoint10[3].Y = 30;	
	tempPoint10[4].X = 100;	
	tempPoint10[4].Y = 20;	
	tempPoint10[5].X = 130;	
	tempPoint10[5].Y = 20;	
	tempPoint10[6].X = 130;	
	tempPoint10[6].Y = 50;
	tempPoint10[7].X = 100;	
	tempPoint10[7].Y = 50;
	tempPoint10[8].X = 80;	
	tempPoint10[8].Y = 50;
	tempPoint10[9].X = 50;	
	tempPoint10[9].Y = 50;
	//printf("LXTestWBS_02_10_19 nCount = %d",nCount);
	ret32 = addDetectRegion( m_TFHandle, (Point *)(&tempPoint10),	nCount );
	if(ret32 < 0)
	{
		printf("ST_12_addDetectRegion_6  ok\n\n");
	}
	else
	{
		printf("ST_12_addDetectRegion_6  ERR\n\n");
	}

	printf("ST_12_addDetectRegion_7 \n");
	nCount = 3;
	Point tempPoint3[3];
	tempPoint3[0].X = 20;	
	tempPoint3[0].Y = 20;	
	tempPoint3[1].X = 40;	
	tempPoint3[1].Y = 20;
	tempPoint3[2].X = 20;	
	tempPoint3[2].Y = 40;
	ret32 = addDetectRegion( m_TFHandle, (Point *)(&tempPoint3),	nCount );
	
	if(0 == ret32 )
	{
		printf("ST_12_addDetectRegion_7 ok\n\n");
	}
	else
	{
		printf("ST_12_addDetectRegion_7  ERR\n\n");
	}

	printf("ST_12_addDetectRegion_8 \n");
	nCount = 3;
	//Point tempPoint3[3];
	tempPoint3[0].X = 2020;	
	tempPoint3[0].Y = 20;	
	tempPoint3[1].X = 1040;	
	tempPoint3[1].Y = 20;
	tempPoint3[2].X = 1020;	
	tempPoint3[2].Y = 40;
	ret32 = addDetectRegion( m_TFHandle, (Point *)(&tempPoint3),	nCount );
	
	if( ret32 < 0  )
	{
		printf("ST_12_addDetectRegion_8 ok\n\n");
	}
	else
	{
		printf("ST_12_addDetectRegion_8  ERR\n\n");
	}



	printf("-------test getDetectConfig ok -----\n");

	return 0;
}


int ST_12_addDetectRegion::uninit()
{
	Driver::uninit();
}

ST_12_addDetectRegion::~ST_12_addDetectRegion()
{
	
}



void ST_12_addDetectRegion::usrCallbackReplyCancalClient1(UINT64 *pReqID,
											  UINT8 *pRet,
											  UINT8 *pReplyData,
											  UINT32 *pDataLen )
{
	printf("---the requestion(%lld) CANCAL CLIENT replied: ret=%d, datalen=%d",
		   *pReqID, *pRet, *pDataLen );
	
}


void ST_12_addDetectRegion::usrCallbackReplyRegisterClient1(UINT64 *pReqID,
												UINT8 *pRet,
												UINT8 *pReplyData,
												UINT32 *pDataLen )
{
	printf("---the requestion(%lld) REGISTER CLIENT replied: ret=%d, datalen=%d",
		   *pReqID, *pRet, *pDataLen );
  
}

void ST_12_addDetectRegion::usrCallbackNotifyServiceInfo1( RWNotifyType *pNotifyType,
											   UINT8 *pExtData,
											   UINT32 *pDataLen)
{
	//printf("---the notify CLIENT INFO type=%d, datalen=%d", *pNotifyType, *pDataLen );
}
void ST_12_addDetectRegion::usrCallbackNotifyFaceInfo1( RWNotifyType *pNotifyType,
											UINT8 *pExtData,
											UINT32 *pDataLen )
{
	printf("---the notify IMG INFO type=%d, datalen=%d",
		   *pNotifyType, *pDataLen );
	
}

	 
	

