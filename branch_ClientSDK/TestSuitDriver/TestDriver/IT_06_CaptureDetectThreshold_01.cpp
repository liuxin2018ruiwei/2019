/********************************************************************
* Name      :  IT_06_CaptureDetectThreshold_01.cpp                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  Liu Xin                                          
* Date      :  2018-8-31                                                
********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "DetectFaceIF.h"
#include "IT_06_CaptureDetectThreshold_01.h"
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


int IT_06_CaptureDetectThreshold_01::init_initDetectFaceIF()
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
	
	
	imgInfo.Width = 1920;
	imgInfo.Height = 1080;
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


int IT_06_CaptureDetectThreshold_01:: init_pCallBackInfo()
{
	callbackInfo.RegisterReplyFunc = usrCallbackReplyRegisterClient2;
	callbackInfo.CancelRelpyFunc = usrCallbackReplyCancalClient2;
	callbackInfo.NotifyFaceFunc = usrCallbackNotifyFaceInfo2;
	printf("callbackInfo.NotifyFaceFunc : %x\n",callbackInfo.NotifyFaceFunc);
	callbackInfo.NotifyClientFunc = usrCallbackNotifyServiceInfo2;
	registerCallback( m_TFHandle, &callbackInfo );
	return 0;
}



IT_06_CaptureDetectThreshold_01::IT_06_CaptureDetectThreshold_01(unsigned int nCaseId ): Driver( nCaseId )
{

}

int IT_06_CaptureDetectThreshold_01::init()
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


/*
void IT_06_CaptureDetectThreshold_01::testCallbackSetLineF( RWNotifyType *pNotifyType, UINT8 *pExtData, UINT32 *pDataLen )
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
*/

void IT_06_CaptureDetectThreshold_01::setline()
{
	CallbackNotifyFunc pFunc = NULL;//testCallbackSetLineF;
	Line line3;
	line3.Head.X = 0;
	line3.Head.Y = 20;
	line3.End.X = 1000;
	line3.End.Y = 20;
	int ret32 = setLine( m_TFHandle, &line3, pFunc);

	Line line2;
	line2.Head.X = 0;
	line2.Head.Y = 220;
	line2.End.X = 1000;
	line2.End.Y = 220;
	ret32 = setLine( m_TFHandle, &line2, pFunc);
	
	printf("pFunc = %x \n",pFunc);
}

void IT_06_CaptureDetectThreshold_01::mySetDetectCfg()
{
	int ret32 = 0;
	DetectCfg cfgData;
	cfgData.ConfigMask = 0x07;
	cfgData.DetectHeartbeat = 0;
	cfgData.DetectType = 0;
	cfgData.DetectThreshold = 5;
	cfgData.ConfigMask |=  0x18;
	cfgData.BackgroundCtrl = 0x12;
	cfgData.FaceCtrl = 0x12;
	ret32 = setDetectConfig(m_TFHandle, &cfgData);
	cfgData.DetectType = 1;
	ret32 = setDetectConfig(m_TFHandle, &cfgData);
}

void IT_06_CaptureDetectThreshold_01::addRegion()
{
	int nCount ; 
	nCount = 4; 
	Point tempPoint1[4];
	tempPoint1[0].X = 1;	
	tempPoint1[0].Y = 1;	
	tempPoint1[1].X = 1001;	
	tempPoint1[1].Y = 1;	
	tempPoint1[2].X = 1001;	
	tempPoint1[2].Y = 1001;	
	tempPoint1[3].X = 1;	
	tempPoint1[3].Y = 1001;	
	addDetectRegion( m_TFHandle, (Point *)(&tempPoint1),  nCount );
	printf("\ninit Face Region\n");
}

void IT_06_CaptureDetectThreshold_01::writeInt32(char* data, int value)
{
	data[0] = char(value & 0xff);
	data[1] = char((value >> 8) & 0xff);
	data[2] = char((value >> 16) & 0xff);
	data[3] = char((value >> 24) & 0xff);

}

void  IT_06_CaptureDetectThreshold_01::myAddFrame()
{
	int ret32 = 0;
	int width = 1920;
	int heigth = 1080;
	FILE *pFile;
	pFile = fopen("/dev/girls.yuv","r+");
	if(pFile == NULL)
	{
		printf("open yuv420sp file failed\n");
		return ;
	}
	printf("init fopen ok\n");
	int len = width * heigth * 1.5;
	char *picData = new  char[len];

	//int imageIdLow = LittleEndianIO::readInt32((const char*)pImage);
	//前面12个是帧头，具体规则是前：
	//byte[0~3]是0xaa
	//bytes[4~7]是一个小端序的32bit值
	//byte[8~11]是0x55
	
	int flag = 0;
	ret32  = fread(picData,width * heigth * 3 / 2, 1, pFile);
	char * p = picData;
	for(int i = 0; i < 4; i++)
	{
		*(p+i) =0xaa;
		*(p + 8 + i)  = 0x55;
	}

   int tempframeNumber = 0xd;


	writeInt32(p+4, tempframeNumber);

	//printf("ID :%04x \n",*((UINT32 *)(p+4)));


	UINT8 * pImage = (UINT8 *)picData;
	addFrame(m_TFHandle, pImage);
	//printf("set pImage = %x \n",pImage);


		
	fclose(pFile);
}
void IT_06_CaptureDetectThreshold_01::initHeatBeat()
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

void IT_06_CaptureDetectThreshold_01::initDetectFace()
{
	int ret32 = 0;
	DetectCfg cfgData;
	cfgData.ConfigMask = 0x01;
	cfgData.DetectType = 0;
	ret32 = setDetectConfig(m_TFHandle, &cfgData);


	sleep(1);

	cfgData.DetectType = 1;
	printf("\n IT_02_detectFace_01	open detect face \n");
	ret32 = setDetectConfig(m_TFHandle, &cfgData);

}

int IT_06_CaptureDetectThreshold_01::faceSize(UINT16 faceSizeValue)
{
	int ret32 = 0;
	DetectCfg cfgData;
	cfgData.ConfigMask = 0x40;
	cfgData.FaceSize = faceSizeValue;
	ret32 = setDetectConfig(m_TFHandle, &cfgData);
	return 0;
}

int IT_06_CaptureDetectThreshold_01:: faceDetectCapture(int type)
{
	int ret32 = 0;
	DetectCfg cfgData;
	cfgData.ConfigMask = 0x01;
	cfgData.DetectType = type;
	ret32 = setDetectConfig(m_TFHandle, &cfgData);
	return 0;
}

int IT_06_CaptureDetectThreshold_01::faceDetectThreshold(int value)
{
	int ret32 = 0;
	DetectCfg cfgData;
	cfgData.ConfigMask = 0x04;
	cfgData.DetectThreshold = value;
	ret32 = setDetectConfig(m_TFHandle, &cfgData);
	return 0;
}
int IT_06_CaptureDetectThreshold_01::run()
{	
	int ret32 = 0;
	int width = 1920;
	int heigth = 1080;
	mySetDetectCfg();
	//setline();
	myAddFrame();

	faceDetectCapture(1);
	//initHeatBeat();
	initDetectFace();
	addRegion();
	faceSize(1);
	
	faceDetectThreshold(51);

while(1);
	
	return 0;
}



int IT_06_CaptureDetectThreshold_01::uninit()
{
	Driver::uninit();
}

IT_06_CaptureDetectThreshold_01::~IT_06_CaptureDetectThreshold_01()
{
	
}



void IT_06_CaptureDetectThreshold_01::usrCallbackReplyCancalClient2(UINT64 *pReqID,
											  UINT8 *pRet,
											  UINT8 *pReplyData,
											  UINT32 *pDataLen )
{
	printf("---the requestion(%lld) CANCAL CLIENT replied: ret=%d, datalen=%d",
		   *pReqID, *pRet, *pDataLen );
	
}


void IT_06_CaptureDetectThreshold_01::usrCallbackReplyRegisterClient2(UINT64 *pReqID,
												UINT8 *pRet,
												UINT8 *pReplyData,
												UINT32 *pDataLen )
{
	printf("---the requestion(%lld) REGISTER CLIENT replied: ret=%d, datalen=%d",
		   *pReqID, *pRet, *pDataLen );
  
}

void IT_06_CaptureDetectThreshold_01::usrCallbackNotifyServiceInfo2( RWNotifyType *pNotifyType,
											   UINT8 *pExtData,
											   UINT32 *pDataLen)
{
	printf("---the notify CLIENT INFO type=%d, datalen=%d", *pNotifyType, *pDataLen );
}


typedef struct _callBackImgData
{
	RWFaceInfo faceInfoData;
	RWImgInfo  imgInfoData;
}CallBackImgData;

static int  m_flag1;

void IT_06_CaptureDetectThreshold_01::usrCallbackNotifyFaceInfo2( RWNotifyType *pNotifyType,
											UINT8 *pExtData,
											UINT32 *pDataLen )
{
	printf("1---[LX ]the notify IMG INFO type=%d, datalen=%d", *pNotifyType, *pDataLen );

	RWNotifyType getRWNotifyType = *pNotifyType;
	printf("getRWNotifyType = %d\n",getRWNotifyType);
	if(NotifyFaceInfo == getRWNotifyType)
	{
		printf("all notify ---->--->-->%x\n",pExtData);
		RWFaceInfo * mRWFaceInfo = (RWFaceInfo *)pExtData ;
		int len = (*pDataLen)/sizeof(RWFaceInfo);
		printf("all notify ok !! get len = %d\n",len);

		for(int i = 0; i < len; i++)
		{
			//printf("mRWFaceInfo[%d] = %x\n",i,mRWFaceInfo[i].TrackNo);
		}
	}
	else if (NotifyImgData == getRWNotifyType)
	{
		printf("2 pic notify---====--- \n\n\n");

		
		/*FILE* pFileW;
		
		char picName[40]  = {0};
		snprintf(picName, 40, "/dev/testPic_%d.jpg", m_flag1);
		
		pFileW = fopen(picName, "wb");

		CallBackImgData *imageData = (CallBackImgData *)pExtData;
		//int len = imageData->imgInfoData.ImgLen;
		int picWidth = imageData->imgInfoData.Width;
		int picHegit = imageData->imgInfoData.Height;
		
		int BGRlen = picWidth * picHegit * 3;
		char *picBGR = new char [BGRlen]; 
		UINT8 *outstream = (UINT8 *)picBGR;
		fwrite(outstream, BGRlen , 1, pFileW);
		
		fclose(pFileW);
		delete[] picBGR;
		*/

		printf("haisi ok notify!");
		m_flag1++;
	}

	
	
	
	
}

	 
	

