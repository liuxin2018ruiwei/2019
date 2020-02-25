/********************************************************************
* Name      :  Driver02_10_156.cpp                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  Zhang Botong                                          
* Date      :  2018-07-02                                                 
********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "DetectFaceIF.h"
#include "Driver02_10_156.h"
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




Driver02_10_156::Driver02_10_156(unsigned int nCaseId ): Driver( nCaseId )
{

}

int Driver02_10_156::init()
{
	Driver::init();
}

int Driver02_10_156::run()
{

	lxTest();
	
	return 0;
}

int Driver02_10_156::uninit()
{
	Driver::uninit();
}

Driver02_10_156::~Driver02_10_156()
{
	
}

/*****************************************
**********************************************************/

 
UINT64 Driver02_10_156::getNowTimeMS()
{
	struct timeval tv;
	gettimeofday(&tv,NULL);
	UINT64 retTime =  (UINT64)((long long)tv.tv_sec*1000 +  (long long)(tv.tv_usec/1000) );//ms
	printf("getNowTimeMS %lu; ms = %ld\n", tv.tv_sec, tv.tv_usec/1000);
	return retTime;
}

void CallbackNotifyFunc1( RWNotifyType *pNotifyType, UINT8 *pExtData, UINT32 *pDataLen )
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
int initTest()
{
	int m_caseID = 11;

	printf("Current Case No: %d Name: null   -------------\n",
		   m_caseID );
	// S1: set the information of the user offered to login
	RWUserInfo userInfo;
	userInfo.EncryptType = 1;
	char userName[UserNameLen]="testUser";
	memcpy( (void*)userInfo.UserName, userName, UserNameLen );
	
	char usrPWD[UserPWDLen]="password";
	memcpy( (void*)userInfo.UserPWD, usrPWD, UserPWDLen );
	
	// S2: set the interface offered to login
	RWIFInfo ifInfo;
	ifInfo.IPType = 0;
	ifInfo.IPArray[0] = 192;
	ifInfo.IPArray[1] = 168;
	ifInfo.IPArray[2] = 1;
	ifInfo.IPArray[3] = 2;
	ifInfo.CtrlPort = 80;
	ifInfo.DataPort = 81;
	
	SerialPortInfo serialInfo;
	char devPath[100] = "/dev/ttyS0";
	serialInfo.DevPathPtr = (UINT8*)devPath;
	serialInfo.PathLen = 100;
	serialInfo.Baudrate = 115200;

	int ImgWidth = 1920;
	int ImgHeight = 1080;
	RWImgInfo imgInfo;
	imgInfo.Width = ImgWidth;
	imgInfo.Height = ImgHeight;
	imgInfo.Step = ImgWidth;
	imgInfo.ImgType = 2;
	
	DetectConfigInfo cfgInfo;
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
	
	// S3: init the detect face SDK 
	void * m_pHandle = NULL;
	int nRet = initDetectFaceIF( &m_pHandle,  &cfgInfo);
	if( nRet != 0 )
	{
		printf("%d %s\n",__LINE__, __func__ );
		return -1;
	}
}

/****==================================================***/
void Driver02_10_156::lxTest()
{
	int a = 10;
/*	
	printf("lxTest relese data!!\n\n");
	uninitDetectFaceIF(&a);
	printf("lxTest relese data!! ok \n\n");

	printf("lxTest init start \n");
	initTest();
	printf("lxTest init start ok\n");
*/

/*
	for(int i = 0; i < 10; i++)
	{
		printf("i = %d\n",i);
		uninitDetectFaceIF(&a);
		initTest();
		sleep(1);
	}

*/

	printf("Driver02_10_156 \n");
	
	int width = 1920;
	int heigth = 1080;

	Line l1;
	l1.Head.X = 12;
	l1.Head.Y = 34;
	l1.End.X = 56;
	l1.End.Y = 720;

	
	CallbackNotifyFunc pFunc =CallbackNotifyFunc1;
	setLine( &a, &l1, pFunc);
	printf("l1 \n\n");

	Line l2;
	l2.Head.X = 18;
	l2.Head.Y = 120;
	l2.End.X = width -1;
	l2.End.Y = 620;
	setLine( &a, &l2, pFunc);
	printf("l2 \n\n");

	Line l3;
	l3.Head.X = 20;
	l3.Head.Y = 20;
	l3.End.X = 56;
	l3.End.Y = 220;
	setLine( &a, &l3, pFunc);
	printf("l3 \n\n");

	Line l4;
	l4.Head.X = 4;
	l4.Head.Y =320;
	l4.End.X = width;
	l4.End.Y = 320;
	setLine( &a, &l4, pFunc);
	printf("l4 \n\n");

	Line l5;
	l5.Head.X = 0;
	l5.Head.Y = 420;
	l5.End.X = width;
	l5.End.Y = 420;
	setLine( &a, &l5, pFunc);
	printf("l5 \n\n");

	
	Line l6;
	l6.Head.X = 0;
	l6.Head.Y = 520;
	l6.End.X = width;
	l6.End.Y = 520;
	setLine( &a, &l6, pFunc);
	printf("l6 \n\n");

	resetCounter(&a);
	//sleep(8);
	//resetCounter(&a);
	printf("\nDriver02_10_156  ok\n\n");


	printf("uninitDetectFaceIF!!\n\n");
	uninitDetectFaceIF(&a);
	printf("uninitDetectFaceIF ok \n\n");
	initTest();
	printf("init ok \n");

/*
	for(int i = 0; i < 10; i++)
	{
		printf("i = %d\n",i);
		uninitDetectFaceIF(&a);
		initTest();
		sleep(1);
	}
	*/

}

void Driver02_10_156::writeInt32(UINT8 * data, int value)
{
	data[0] =  (char)(value & 0xff);
	data[1] =  (char)((value >> 8) & 0xff);
	data[2] =  (char)((value >> 16) & 0xff);
	data[3] =  (char)((value >> 24) & 0xff);
	
}

	 
	

