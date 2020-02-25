/********************************************************************
* Name      :  ST_16_startUpgrade.cpp                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  Liu Xin                                          
* Date      :  2018-8-22                                                
********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "DetectFaceIF.h"
#include "ST_16_startUpgrade.h"
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


int ST_16_startUpgrade::init_initDetectFaceIF()
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


int ST_16_startUpgrade:: init_pCallBackInfo()
{
	callbackInfo.RegisterReplyFunc = usrCallbackReplyRegisterClient1;
	callbackInfo.CancelRelpyFunc = usrCallbackReplyCancalClient1;
	callbackInfo.NotifyFaceFunc = usrCallbackNotifyFaceInfo1;
	callbackInfo.NotifyClientFunc = usrCallbackNotifyServiceInfo1;
	registerCallback( m_TFHandle, &callbackInfo );
	return 0;
}



ST_16_startUpgrade::ST_16_startUpgrade(unsigned int nCaseId ): Driver( nCaseId )
{

}

int ST_16_startUpgrade::init()
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

void testCallBackUpgrade(UINT64 *pReqID, UINT8 *pRet, UINT8 *pReplyData,UINT32 *pDataLen )
{
	printf("for fate!\n\n");
}

int ST_16_startUpgrade::upgradeAppCfg()
{
	int ret32 = 0;

	UINT64 pReqID = 56;
	UpgradeDataInfo DataInfo ;
	UpgradeDataInfo *pDataInfo = &DataInfo;
	const char* pFileName =  "app.cfg";//"Camera-QSPI-image.bin";
	pDataInfo->FileNamPtr = (UINT8*)pFileName;
	pDataInfo->NameLen = sizeof("app.cfg");//sizeof("Camera-QSPI-image.bin");
	const char* pFilePath = "/home"; 
	pDataInfo->FilePathPtr = (UINT8*)pFilePath;
	pDataInfo->PathLen = sizeof("/home");
	pDataInfo->UpgradeType = 0x00;//升级类型,0:app.cfg, 1:app.pkg, 2:Camera-QSPI-image.bin
	UINT8 tempMD5[16] = { 	0x34,0x21,0x08,0x35,
							0x5a,0xad,0xdb,0xeb,
							0xaa,0xb7,0xd7,0xae,
							0xd8,0xf3,0x10,0x99  };

	printf("FilePathPtr = %s;name FileNamPtr = %s\n",pDataInfo->FileNamPtr,pDataInfo->FilePathPtr);

	memcpy(&(pDataInfo->MD5), &tempMD5,16);
	
	pDataInfo->FileSize = 1322;//,0:app.cfg
	
	CallbackReplyFunc pFuncTest = testCallBackUpgrade; 
	printf("\n%x\n",pFuncTest);

	printf(" pDataInfo = %x,pDataInfo->FileSize = %u \n",pDataInfo,pDataInfo->FileSize);
	return startUpgrade(m_TFHandle, &pReqID, pDataInfo,pFuncTest);
}

int ST_16_startUpgrade::upgradeAppPkg()
{
	int ret32 = 0;

	UINT64 pReqID = 56;
	UpgradeDataInfo DataInfo ;
	UpgradeDataInfo *pDataInfo = &DataInfo;
	const char* pFileName =  "app.pkg";//"Camera-QSPI-image.bin";
	pDataInfo->FileNamPtr = (UINT8*)pFileName;
	pDataInfo->NameLen = sizeof("app.pkg");//sizeof("Camera-QSPI-image.bin");
	const char* pFilePath = "/dev"; 
	pDataInfo->FilePathPtr = (UINT8*)pFilePath;
	pDataInfo->PathLen = sizeof("/dev");
	pDataInfo->UpgradeType = 1;//升级类型,0:app.cfg, 1:app.pkg, 2:Camera-QSPI-image.bin


	UINT8 tempMD5[16] ={ 	0xf4,0x07,0x90,0x05,
							0xa8,0xe3,0x01,0xbb,
							0xd8,0x1d,0x20,0x14,
							0xd8,0x89,0x2f,0xac }; 
	memcpy(&(pDataInfo->MD5), &tempMD5,16);

	pDataInfo->FileSize = 1402696;

	printf("FilePathPtr = %s;name FileNamPtr = %s\n",pDataInfo->FileNamPtr,pDataInfo->FilePathPtr);

	
	CallbackReplyFunc pFuncTest = testCallBackUpgrade; 
	//printf("\n%x\n",pFuncTest);

	printf(" pDataInfo = %x,pDataInfo->FileSize = %u \n",pDataInfo,pDataInfo->FileSize);
	return startUpgrade(m_TFHandle, &pReqID, pDataInfo,pFuncTest);
}


int ST_16_startUpgrade::upgradeCamereaQSPI()
{
	int ret32 = 0;

	UINT64 pReqID = 56;
	UpgradeDataInfo DataInfo ;
	UpgradeDataInfo *pDataInfo = &DataInfo;
	const char* pFileName =  "Camera-QSPI-image.bin";
	pDataInfo->FileNamPtr = (UINT8*)pFileName;
	pDataInfo->NameLen = sizeof("Camera-QSPI-image.bin");
	const char* pFilePath = "/dev"; 
	pDataInfo->FilePathPtr = (UINT8*)pFilePath;
	pDataInfo->PathLen = sizeof("/dev");
	pDataInfo->UpgradeType = 2;//升级类型,0:app.cfg, 1:app.pkg, 2:Camera-QSPI-image.bin

	UINT8 tempMD5[16] ={    0x28,0xeb,0x7b,0x74,
							0x3e,0x87,0x93,0xd6,
							0x20,0xf2,0xfd,0x3a,
							0x97,0xa4,0x85,0xdc }; 
	memcpy(&(pDataInfo->MD5), &tempMD5,16);

	pDataInfo->FileSize = 6645704;

	printf("FilePathPtr = %s;name FileNamPtr = %s\n",pDataInfo->FileNamPtr,pDataInfo->FilePathPtr);

	
	CallbackReplyFunc pFuncTest = testCallBackUpgrade; 
	//printf("\n%x\n",pFuncTest);

	printf(" pDataInfo = %x,pDataInfo->FileSize = %u \n",pDataInfo,pDataInfo->FileSize);
	return startUpgrade(m_TFHandle, &pReqID, pDataInfo,pFuncTest);
}

int ST_16_startUpgrade::getpDataCfgInfo( )
{
	//pDataInfo = &m_upgradeDataInfo;

	int ret32 = 0;
	UINT64 pReqID = 56;
	const char* pFileName =  "app.cfg";//"Camera-QSPI-image.bin";
	m_upgradeDataInfo.FileNamPtr = (UINT8*)pFileName;
	m_upgradeDataInfo.NameLen = sizeof("app.cfg");//sizeof("Camera-QSPI-image.bin");
	const char* pFilePath = "/home"; 
	m_upgradeDataInfo.FilePathPtr = (UINT8*)pFilePath;
	m_upgradeDataInfo.PathLen = sizeof("/home");
	m_upgradeDataInfo.UpgradeType = 0x00;//升级类型,0:app.cfg, 1:app.pkg, 2:Camera-QSPI-image.bin
	UINT8 tempMD5[16] = { 	0x34,0x21,0x08,0x35,
							0x5a,0xad,0xdb,0xeb,
							0xaa,0xb7,0xd7,0xae,
							0xd8,0xf3,0x10,0x99  };

	printf("FilePathPtr = %s;name FileNamPtr = %s\n",m_upgradeDataInfo.FileNamPtr,m_upgradeDataInfo.FilePathPtr);
	memcpy(&(m_upgradeDataInfo.MD5), &tempMD5,16);
	m_upgradeDataInfo.FileSize = 1322;//
	m_upgradeDataInfo.UpgradeType = 0;
	

	printf(" pDataInfo = %x,pDataInfo->FileSize = %u \n",&m_upgradeDataInfo,m_upgradeDataInfo.FileSize);
	return 0;
}


int ST_16_startUpgrade::run()
{

	if(NULL == m_TFHandle)
	{
		printf("get m_TFHandle ERR !\n");
		return -1;
	}
	//upgradeAppCfg();
	//upgradeAppPkg();
	//upgradeCamereaQSPI();

	UINT64 pReqID = 56;
	UpgradeDataInfo *pDataInfo;
	//CallbackReplyFunc pFuncTest = testCallBackUpgrade;
	CallbackReplyFunc pFuncTestNULL = NULL;
	CallbackReplyFunc pFuncTest = testCallBackUpgrade; 

	
	printf("ST_16_startUpgrade_1 \n");
	int ret32 = startUpgrade( NULL, NULL, NULL, NULL);
	if(ret32 < 0)
	{
		printf("ST_16_startUpgrade_1  ok\n\n");
	}else{
		printf("ST_16_startUpgrade_1  ERR\n\n");
	}

	printf("ST_16_startUpgrade_2 \n");
	 ret32 = startUpgrade( m_TFHandle, NULL, NULL, NULL);
	if(ret32 < 0)
	{
		printf("ST_16_startUpgrade_2  ok\n\n");
	}else{
		printf("ST_16_startUpgrade_2  ERR\n\n");
	}

	printf("ST_16_startUpgrade_3 \n");
	 ret32 = startUpgrade( m_TFHandle, &pReqID, NULL, NULL);
	if(ret32 < 0)
	{
		printf("ST_16_startUpgrade_3  ok\n\n");
	}else{
		printf("ST_16_startUpgrade_3  ERR\n\n");
	}

	
	printf("ST_16_startUpgrade_4 \n");
	getpDataCfgInfo();
	printf("Test startupgrade start\n");
	sleep(1);
	printf("type = %d, name = %s \n",m_upgradeDataInfo.UpgradeType, m_upgradeDataInfo.FileNamPtr);
	ret32 = startUpgrade( m_TFHandle, &pReqID, &m_upgradeDataInfo, NULL);
	if(ret32 == 0)
	{
		printf("ST_16_startUpgrade_4  ok\n\n");
	}else{
		printf("ST_16_startUpgrade_4  ERR\n\n");
	}
	
while(1){;};

	sleep(10);	

	printf("ST_16_startUpgrade_5 \n");
	//getpDataCfgInfo();
	//int ret32 = startUpgrade( m_TFHandle, &pReqID, pDataInfo, pFuncTest);
	sleep(1);
	ret32 = upgradeAppCfg();
	if(ret32 == 0)
	{
		printf("ST_16_startUpgrade_5  ok\n\n");
	}else{
		printf("ST_16_startUpgrade_5  ERR\n\n");
	}

	sleep(10);


	printf("ST_16_startUpgrade_6 \n");
	//getpDataCfgInfo();
	//int ret32 = startUpgrade( m_TFHandle, &pReqID, pDataInfo, pFuncTest);
	ret32 = upgradeAppPkg();
	if(ret32 == 0)
	{
		printf("ST_16_startUpgrade_6  ok\n\n");
	}else{
		printf("ST_16_startUpgrade_6  ERR\n\n");
	}

	sleep(10);


	printf("ST_16_startUpgrade_7 \n");
	//getpDataCfgInfo();
	//int ret32 = startUpgrade( m_TFHandle, &pReqID, pDataInfo, pFuncTest);
	ret32 = upgradeCamereaQSPI();
	if(ret32 == 0)
	{
		printf("ST_16_startUpgrade_7  ok\n\n");
	}else{
		printf("ST_16_startUpgrade_7  ERR\n\n");
	}

	sleep(10);

	printf("ST_16_startUpgrade_8 \n");
	getpDataCfgInfo();
	m_upgradeDataInfo.FileNamPtr = NULL;
	 ret32 = startUpgrade( m_TFHandle, &pReqID, &m_upgradeDataInfo, pFuncTest);
	if(ret32 < 0)
	{
		printf("ST_16_startUpgrade_8  ok\n\n");
	}else{
		printf("ST_16_startUpgrade_8  ERR\n\n");
	}

	sleep(10);

	printf("ST_16_startUpgrade_9 \n");
	getpDataCfgInfo();
	m_upgradeDataInfo.NameLen = 0;
	 ret32 = startUpgrade( m_TFHandle, &pReqID, &m_upgradeDataInfo, pFuncTest);
	if(ret32 < 0)
	{
		printf("ST_16_startUpgrade_9  ok\n\n");
	}else{
		printf("ST_16_startUpgrade_9  ERR\n\n");
	}

	sleep(10);

	printf("ST_16_startUpgrade_10 \n");
	getpDataCfgInfo();
	m_upgradeDataInfo.FilePathPtr = NULL;
	 ret32 = startUpgrade( m_TFHandle, &pReqID, &m_upgradeDataInfo, pFuncTest);
	if(ret32 < 0)
	{
		printf("ST_16_startUpgrade_10 ok\n\n");
	}else{
		printf("ST_16_startUpgrade_10  ERR\n\n");
	}

	sleep(10);

	printf("ST_16_startUpgrade_11 \n");
	getpDataCfgInfo();
	m_upgradeDataInfo.PathLen = 0;
	 ret32 = startUpgrade( m_TFHandle, &pReqID, &m_upgradeDataInfo, pFuncTest);
	if(ret32 < 0)
	{
		printf("ST_16_startUpgrade_11 ok\n\n");
	}else{
		printf("ST_16_startUpgrade_11  ERR\n\n");
	}

	sleep(10);


	printf("ST_16_startUpgrade_12 \n");
	getpDataCfgInfo();
	memset(&(m_upgradeDataInfo.MD5), 0, 16);
	 ret32 = startUpgrade( m_TFHandle, &pReqID, &m_upgradeDataInfo, pFuncTest);
	if(ret32 < 0)
	{
		printf("ST_16_startUpgrade_12 ok\n\n");
	}else{
		printf("ST_16_startUpgrade_12  ERR\n\n");
	}

	sleep(10);


	printf("ST_16_startUpgrade_13 \n");
	getpDataCfgInfo();
	m_upgradeDataInfo.FileNamPtr = (UINT8*)"123";
	 ret32 = startUpgrade( m_TFHandle, &pReqID, &m_upgradeDataInfo, pFuncTest);
	if(ret32 < 0)
	{
		printf("ST_16_startUpgrade_13 ok\n\n");
	}else{
		printf("ST_16_startUpgrade_13  ERR\n\n");
	}

	sleep(10);

	printf("ST_16_startUpgrade_14 \n");
	getpDataCfgInfo();
	m_upgradeDataInfo.NameLen = 1;
	 ret32 = startUpgrade( m_TFHandle, &pReqID, &m_upgradeDataInfo, pFuncTest);
	if(ret32 < 0)
	{
		printf("ST_16_startUpgrade_14 ok\n\n");
	}else{
		printf("ST_16_startUpgrade_14  ERR\n\n");
	}

	sleep(10);

	printf("ST_16_startUpgrade_15 \n");
	getpDataCfgInfo();
	m_upgradeDataInfo.FilePathPtr = (UINT8*)"1234567890123456789012345678901234567890/123456789012345";//56
	ret32 = startUpgrade( m_TFHandle, &pReqID, &m_upgradeDataInfo, pFuncTest);
	if(ret32 == 0)
	{
		printf("ST_16_startUpgrade_15 ok\n\n");
	}else{
		printf("ST_16_startUpgrade_15  ERR\n\n");
	}

	sleep(10);


	printf("ST_16_startUpgrade_16 \n");
	getpDataCfgInfo();
	m_upgradeDataInfo.PathLen = 1;
	ret32 = startUpgrade( m_TFHandle, &pReqID, &m_upgradeDataInfo, pFuncTest);
	if(ret32 < 0)
	{
		printf("ST_16_startUpgrade_16 ok\n\n");
	}else{
		printf("ST_16_startUpgrade_16  ERR\n\n");
	}

	sleep(10);

	printf("ST_16_startUpgrade_17 \n");
	getpDataCfgInfo();
	memset(&(m_upgradeDataInfo.MD5), 1, 16);
	 ret32 = startUpgrade( m_TFHandle, &pReqID, &m_upgradeDataInfo, pFuncTest);
	if(ret32 < 0)
	{
		printf("ST_16_startUpgrade_17 ok\n\n");
	}else{
		printf("ST_16_startUpgrade_17  ERR\n\n");
	}


	sleep(10);

	printf("ST_16_startUpgrade_18 \n");
	getpDataCfgInfo();
	m_upgradeDataInfo.NameLen = 123;
	 ret32 = startUpgrade( m_TFHandle, &pReqID, &m_upgradeDataInfo, pFuncTest);
	if(ret32 < 0)
	{
		printf("ST_16_startUpgrade_18 ok\n\n");
	}else{
		printf("ST_16_startUpgrade_18  ERR\n\n");
	}


	sleep(10);

	printf("ST_16_startUpgrade_19 \n");
	getpDataCfgInfo();
	m_upgradeDataInfo.PathLen = 123;
	 ret32 = startUpgrade( m_TFHandle, &pReqID, &m_upgradeDataInfo, pFuncTest);
	if(ret32 < 0)
	{
		printf("ST_16_startUpgrade_19 ok\n\n");
	}else{
		printf("ST_16_startUpgrade_19  ERR\n\n");
	}

	sleep(10);

	printf("ST_16_startUpgrade_20 \n");
	getpDataCfgInfo();
	m_upgradeDataInfo.UpgradeType = 6;
	 ret32 = startUpgrade( m_TFHandle, &pReqID, &m_upgradeDataInfo, pFuncTest);
	if(ret32 < 0)
	{
		printf("ST_16_startUpgrade_20 ok\n\n");
	}else{
		printf("ST_16_startUpgrade_20  ERR\n\n");
	}
	sleep(10);
	
	while(1){};
	printf("-------test ST_16_startUpgrade ok -----\n");


	
	return 0;
}



int ST_16_startUpgrade::uninit()
{
	Driver::uninit();
}

ST_16_startUpgrade::~ST_16_startUpgrade()
{
	
}



void ST_16_startUpgrade::usrCallbackReplyCancalClient1(UINT64 *pReqID,
											  UINT8 *pRet,
											  UINT8 *pReplyData,
											  UINT32 *pDataLen )
{
	printf("---the requestion(%lld) CANCAL CLIENT replied: ret=%d, datalen=%d",
		   *pReqID, *pRet, *pDataLen );
	
}


void ST_16_startUpgrade::usrCallbackReplyRegisterClient1(UINT64 *pReqID,
												UINT8 *pRet,
												UINT8 *pReplyData,
												UINT32 *pDataLen )
{
	printf("---the requestion(%lld) REGISTER CLIENT replied: ret=%d, datalen=%d",
		   *pReqID, *pRet, *pDataLen );
  
}

void ST_16_startUpgrade::usrCallbackNotifyServiceInfo1( RWNotifyType *pNotifyType,
											   UINT8 *pExtData,
											   UINT32 *pDataLen)
{
	//printf("---the notify CLIENT INFO type=%d, datalen=%d", *pNotifyType, *pDataLen );
}
void ST_16_startUpgrade::usrCallbackNotifyFaceInfo1( RWNotifyType *pNotifyType,
											UINT8 *pExtData,
											UINT32 *pDataLen )
{
	printf("---the notify IMG INFO type=%d, datalen=%d",
		   *pNotifyType, *pDataLen );
	
}

	 
	

