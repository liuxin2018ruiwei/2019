/********************************************************************
* Name      :  IT_15_UpgradeFunction_01.cpp                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  Liu Xin                                          
* Date      :  2018-8-22                                                
********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "DetectFaceIF.h"
#include "IT_15_UpgradeFunction_01.h"
#include <unistd.h>
#include <string>
#include <cstring>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <memory.h>

#include <unistd.h>
#include "DetectFaceIFCommon.h"




// to_string example
#include <iostream>   // std::cout
#include <string>     // std::string, std::to_string

using namespace std;


int IT_15_UpgradeFunction_01::init_initDetectFaceIF()
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


int IT_15_UpgradeFunction_01:: init_pCallBackInfo()
{
	callbackInfo.RegisterReplyFunc = usrCallbackReplyRegisterClient1;
	callbackInfo.CancelRelpyFunc = usrCallbackReplyCancalClient1;
	callbackInfo.NotifyFaceFunc = usrCallbackNotifyFaceInfo1;
	callbackInfo.NotifyClientFunc = usrCallbackNotifyServiceInfo1;
	registerCallback( m_TFHandle, &callbackInfo );
	return 0;
}



IT_15_UpgradeFunction_01::IT_15_UpgradeFunction_01(unsigned int nCaseId ): Driver( nCaseId )
{

}

int IT_15_UpgradeFunction_01::init()
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

void IT_15_UpgradeFunction_01_CallBackUpgrade(UINT64 *pReqID, UINT8 *pRet, UINT8 *pReplyData,UINT32 *pDataLen )
{
	UINT8 getReplyData  =*pReplyData;
	int data = getReplyData;
	printf("3getReplyData = %d, %X\n",data, getReplyData);
	if(0  == getReplyData)
	{
		printf("start upgrade\n");
	}
	if(UpgradeReqType :: ReqStopUpgrade  == getReplyData)
	{
		printf("end upgrade\n");
	}
	printf("for fate!\n\n");
}

int IT_15_UpgradeFunction_01::upgradeAppCfg()
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
	
	CallbackReplyFunc pFuncTest = IT_15_UpgradeFunction_01_CallBackUpgrade; 
	printf("\n%x\n",pFuncTest);

	printf(" pDataInfo = %x,pDataInfo->FileSize = %u \n",pDataInfo,pDataInfo->FileSize);
	return startUpgrade(m_TFHandle, &pReqID, pDataInfo,pFuncTest);
}

int IT_15_UpgradeFunction_01::upgradeAppPkg()
{
	int ret32 = 0;

	UINT64 pReqID = 56;
	UpgradeDataInfo DataInfo ;
	UpgradeDataInfo *pDataInfo = &DataInfo;
	const char* pFileName =  "app.pkg";//"Camera-QSPI-image.bin";
	pDataInfo->FileNamPtr = (UINT8*)pFileName;
	pDataInfo->NameLen = sizeof("app.pkg");//sizeof("Camera-QSPI-image.bin");
	const char* pFilePath = "/home"; 
	pDataInfo->FilePathPtr = (UINT8*)pFilePath;
	pDataInfo->PathLen = sizeof("/home");
	pDataInfo->UpgradeType = 1;//升级类型,0:app.cfg, 1:app.pkg, 2:Camera-QSPI-image.bin


	UINT8 tempMD5[16] ={ 	0x87,0xdd,0xca,0xeb,
							0xc3,0x19,0x59,0x44,
							0x6e,0x39,0x59,0x2b,
							0x31,0xe9,0x67,0x08 }; 
	memcpy(&(pDataInfo->MD5), &tempMD5,16);

	pDataInfo->FileSize = 1028816;

	printf("FilePathPtr = %s;name FileNamPtr = %s\n",pDataInfo->FileNamPtr,pDataInfo->FilePathPtr);

	
	CallbackReplyFunc pFuncTest = IT_15_UpgradeFunction_01_CallBackUpgrade; 
	//printf("\n%x\n",pFuncTest);

	sleep(2);
	printf(" pDataInfo = %x,pDataInfo->FileSize = %u \n",pDataInfo,pDataInfo->FileSize);
	return startUpgrade(m_TFHandle, &pReqID, pDataInfo,pFuncTest);
}


int IT_15_UpgradeFunction_01::upgradeCamereaQSPI()
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

	
	CallbackReplyFunc pFuncTest = IT_15_UpgradeFunction_01_CallBackUpgrade; 
	//printf("\n%x\n",pFuncTest);

	printf(" pDataInfo = %x,pDataInfo->FileSize = %u \n",pDataInfo,pDataInfo->FileSize);
	return startUpgrade(m_TFHandle, &pReqID, pDataInfo,pFuncTest);
}

int IT_15_UpgradeFunction_01::getpDataCfgInfo( )
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
void IT_15_UpgradeFunction_01::qurySoftVersion()
{
	VersionInfo testVersionInfo ;
	int ret32 = getFaceDetectVersionInfo(m_TFHandle, &testVersionInfo);
		if(ret32 == 0)
		{
			printf("%s \n  ProtocolVersion[0] = %d, ProtocolVersion[1] = %d\n %s\n",
				testVersionInfo.FirmwareVersion,
				testVersionInfo.ProtocolVersion[0],testVersionInfo.ProtocolVersion[1],testVersionInfo.SoftwareVersion);
		}else{
			printf("qurySoftVersion  ERR\n");
		}

}

void IT_15_UpgradeFunction_01::initHeatBeat()
{
	int ret32 = 0;
	DetectCfg cfgData;
	cfgData.ConfigMask = 0x02;

	printf("init heartBeat\n");
	
	cfgData.DetectHeartbeat = 10;
	ret32 = setDetectConfig(m_TFHandle, &cfgData);
	if(ret32 == 0)
	{
		printf("open send 0x0102 = %d \n",cfgData.DetectHeartbeat);
	}
}

int IT_15_UpgradeFunction_01::run()
{
	//initHeatBeat();
	qurySoftVersion();

	sleep(1);
	upgradeAppPkg();
	
	while(1)
	{
		//sleep(10);
		//printf("run sleep 10s\n");
	};
	return 0;
}



int IT_15_UpgradeFunction_01::uninit()
{
	Driver::uninit();
}

IT_15_UpgradeFunction_01::~IT_15_UpgradeFunction_01()
{
	
}



void IT_15_UpgradeFunction_01::usrCallbackReplyCancalClient1(UINT64 *pReqID,
											  UINT8 *pRet,
											  UINT8 *pReplyData,
											  UINT32 *pDataLen )
{
	printf("---the requestion(%lld) CANCAL CLIENT replied: ret=%d, datalen=%d",
		   *pReqID, *pRet, *pDataLen );
	
}


void IT_15_UpgradeFunction_01::usrCallbackReplyRegisterClient1(UINT64 *pReqID,
												UINT8 *pRet,
												UINT8 *pReplyData,
												UINT32 *pDataLen )
{
	printf("---the requestion(%lld) REGISTER CLIENT replied: ret=%d, datalen=%d",
		   *pReqID, *pRet, *pDataLen );
  
}

void IT_15_UpgradeFunction_01::usrCallbackNotifyServiceInfo1( RWNotifyType *pNotifyType,
											   UINT8 *pExtData,
											   UINT32 *pDataLen)
{
	//printf("---the notify CLIENT INFO type=%d, datalen=%d", *pNotifyType, *pDataLen );
}
void IT_15_UpgradeFunction_01::usrCallbackNotifyFaceInfo1( RWNotifyType *pNotifyType,
											UINT8 *pExtData,
											UINT32 *pDataLen )
{
	//printf("---the notify IMG INFO type=%d, datalen=%d",
		  // *pNotifyType, *pDataLen );
	
}

	 
	

