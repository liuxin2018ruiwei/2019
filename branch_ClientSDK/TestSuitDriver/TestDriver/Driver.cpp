/********************************************************************
* Name      :  Driver.cpp                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  Zhang Botong                                          
* Date      :  2018-07-02                                                 
********************************************************************/

#include<string.h>
#include<stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "Driver.h"
#include "DetectFaceIF.h"

Driver::Driver(unsigned int caseID ):
	m_caseID( caseID),m_pHandle(nullptr ), RequestID(0), ImgWidth(1920),
	ImgHeight(1080),DummyFrame(nullptr )
{
}

int Driver::init()
{
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

	static int sg_baudrate = 115200;
	cfgInfo.SerialPortInfoPtr->Baudrate = sg_baudrate;
	const char *uart_dev = "/dev/ttyAMA2";
	cfgInfo.SerialPortInfoPtr->DevPathPtr = (UINT8 *)uart_dev;
	cfgInfo.SerialPortInfoPtr->PathLen = sizeof("/dev/ttyAMA2");
	printf("Driver -->uart_dev = %s, Baudrate = %d,DevPathPtr = %s,len = %d\n",
										uart_dev,
											cfgInfo.SerialPortInfoPtr->Baudrate ,
												cfgInfo.SerialPortInfoPtr->DevPathPtr,
													cfgInfo.SerialPortInfoPtr->PathLen);
	// S3: init the detect face SDK 
	m_pHandle = NULL;
	int nRet = initDetectFaceIF( &m_pHandle,  &cfgInfo);
	if( nRet != 0 )
	{
		printf("ERR %d %s\n",__LINE__, __func__ );
		return -1;
	}

	//S4: set some necessary call back function to receive data
	CallBackInfo callbackInfo;
	callbackInfo.RegisterReplyFunc = usrCallbackReplyRegisterClient;
	callbackInfo.CancelRelpyFunc = usrCallbackReplyCancalClient;
	callbackInfo.NotifyFaceFunc = usrCallbackNotifyFaceInfo;
	callbackInfo.NotifyClientFunc = usrCallbackNotifyServiceInfo;

	nRet = registerCallback( m_pHandle, &callbackInfo );
	if( nRet != 0 )
	{
		printf("ERR %d %s\n",__LINE__, __func__ );
		return -2;
	}
	
	//S5: start SDK to transort face
	nRet = startSDK( m_pHandle );
	if( nRet != 0)
	{
		printf("ERR %d %s\n",__LINE__, __func__ );
		return -3;
	}
	
	return 0;
}
unsigned int Driver::getCaseID()
{
	return m_caseID;
}

int Driver::run()
{
	//S6: some normal work
  
	char input = 'r';
	printf("-----------------------------------------\n");
	printf("q: quit\n");
	printf("register the client\n");
	register_Client();
	printf("cancal the client\n");
	cancal_Client();
	printf("get the client version\n");
	getClientVersion();
	printf("get the service version\n");
	getServiceVersion();


	int nCount = 0;
	while( input != 'q' )
	{
	  usleep( 500 * 1000 );

	  add_Frame();
	  nCount++;
	  //if( ( nCount % 20 ) == 0 )
	  {
		printf("run more 10 second? anyKey :continue, q:quit\n");
		input = getchar( );
		fflush(stdin );
		nCount = 0;
	  }
	}

	return 0;
}

int Driver::uninit()
{
	//S7: stop sdk 
	stopSDK( m_pHandle );

	
	//S8: uninit sdk
	uninitDetectFaceIF( m_pHandle );
	
	return 0;
}

int Driver::register_Client()
{
	// S1: set the information of the user offered to login
	RWUserInfo userInfo;
	userInfo.EncryptType = 1;
	char userName[UserNameLen]="ClientUser";
	memcpy( (void*)userInfo.UserName, userName, UserNameLen );

	char usrPWD[UserPWDLen]="password";
	memcpy( (void*)userInfo.UserPWD, usrPWD, UserPWDLen );

	// S2: set the interface offered to login
	RWIFInfo ifInfo;
	ifInfo.IPType = 0;
	ifInfo.IPArray[0] = 192;
	ifInfo.IPArray[1] = 168;
	ifInfo.IPArray[2] = 1;
	ifInfo.IPArray[3] = 3;
	ifInfo.CtrlPort = 80;
	ifInfo.DataPort = 81;
	ifInfo.CameraID = 1;
	// S3: generate a request id
	RequestID++;
	// S4: send the request
	registerClient( m_pHandle, &RequestID, &ifInfo, &userInfo );

	printf("registwer request: reqid=%lld, client username=%s, userPwd=%d, clientip=%d.%d.%d.%d, ctrlport=%d, dataport=%d\n",
		   RequestID, userName, usrPWD,
		   ifInfo.IPArray[0], ifInfo.IPArray[1],
		   ifInfo.IPArray[2], ifInfo.IPArray[3],
		   ifInfo.CtrlPort, ifInfo.DataPort );
  
	return 0;
}

int Driver::cancal_Client()
{
	// S1: set the information of the user offered to login
	RWUserInfo userInfo;
	userInfo.EncryptType = 1;
	char userName[UserNameLen]="ClientUser";
	memcpy( (void*)userInfo.UserName, userName, UserNameLen );

	char usrPWD[UserPWDLen]="password";
	memcpy( userInfo.UserPWD, usrPWD, UserPWDLen );

	// S2: set the interface offered to login
	RWIFInfo ifInfo;
	ifInfo.IPType = 0;
	ifInfo.IPArray[0] = 192;
	ifInfo.IPArray[1] = 168;
	ifInfo.IPArray[2] = 1;
	ifInfo.IPArray[3] = 3;
	ifInfo.CtrlPort = 80;
	ifInfo.DataPort = 81;
	ifInfo.CameraID = 1;
	// S3: generate a request id
	RequestID++;
	// S4: send the request
	cancalClient( m_pHandle, &RequestID, &ifInfo, &userInfo );

	printf("cancal request: reqid=%lld, client username=%s, userPwd=%d, clientip=%d.%d.%d.%d, ctrlport=%d, dataport=%d\n",
		   RequestID, userName, usrPWD,
		   ifInfo.IPArray[0], ifInfo.IPArray[1],
		   ifInfo.IPArray[2], ifInfo.IPArray[3],
		   ifInfo.CtrlPort, ifInfo.DataPort );
  
	return 0;
}

int Driver::getServiceVersion()
{
	RequestID++;
	char temp[2] ={0};
	getServiceSDKVersion( m_pHandle, temp, 2 ); 

	printf("the version will send to notify\n");
	return 0;

}

int	Driver::getClientVersion()
{
	char version[100];
	memset( version, 0, 100 );
  
	getClientSDKVersion( m_pHandle, (UINT8*)version, 100 );

	printf("the Client service Verson: %s\n", version );

	return 0;
}

int Driver::add_Frame()
{
	if( NULL == DummyFrame )
	{
		DummyFrame = (char*)malloc( ImgWidth * ImgHeight * 1.5  );
		if( NULL == DummyFrame )
		{
			return -1;
		}
	  
		//readFile
	}

	addFrame( m_pHandle, (UINT8*)DummyFrame );

	return 0;
}
void Driver::usrCallbackReplyCancalClient(UINT64 *pReqID,
											  UINT8 *pRet,
											  UINT8 *pReplyData,
											  UINT32 *pDataLen )
{
	printf("---the requestion(%lld) CANCAL CLIENT replied: ret=%d, datalen=%d",
		   *pReqID, *pRet, *pDataLen );
	
}


void Driver::usrCallbackReplyRegisterClient(UINT64 *pReqID,
												UINT8 *pRet,
												UINT8 *pReplyData,
												UINT32 *pDataLen )
{
	printf("---the requestion(%lld) REGISTER CLIENT replied: ret=%d, datalen=%d",
		   *pReqID, *pRet, *pDataLen );
  
}

void Driver::usrCallbackNotifyServiceInfo( RWNotifyType *pNotifyType,
											   UINT8 *pExtData,
											   UINT32 *pDataLen)
{
	//printf("---the notify CLIENT INFO type=%d, datalen=%d", *pNotifyType, *pDataLen );
}
void Driver::usrCallbackNotifyFaceInfo( RWNotifyType *pNotifyType,
											UINT8 *pExtData,
											UINT32 *pDataLen )
{
	printf("---the notify IMG INFO type=%d, datalen=%d",
		   *pNotifyType, *pDataLen );
	
}

void *Driver::getHandle()
{
	return m_pHandle;
}


int Driver::setHandle(void* pHandle)
{
	m_pHandle = pHandle;

	return 0;
}

Driver::~Driver()
{
}
