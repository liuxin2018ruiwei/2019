/********************************************************************
* Name      :  ST_11_setDetectConfig.cpp                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  Liu Xin                                          
* Date      :  2018-8-22                                                
********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "DetectFaceIF.h"
#include "ST_11_setDetectConfig.h"
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


int ST_11_setDetectConfig::init_initDetectFaceIF()
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


int ST_11_setDetectConfig:: init_pCallBackInfo()
{
	callbackInfo.RegisterReplyFunc = usrCallbackReplyRegisterClient1;
	callbackInfo.CancelRelpyFunc = usrCallbackReplyCancalClient1;
	callbackInfo.NotifyFaceFunc = usrCallbackNotifyFaceInfo1;
	callbackInfo.NotifyClientFunc = usrCallbackNotifyServiceInfo1;
	void *TFHandle = (void*)1;
	registerCallback( m_TFHandle, &callbackInfo );
	return 0;
}



ST_11_setDetectConfig::ST_11_setDetectConfig(unsigned int nCaseId ): Driver( nCaseId )
{

}

int ST_11_setDetectConfig::init()
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


void ST_11_setDetectConfig::initSetCfg(DetectCfg * pSetCfg)
{
	pSetCfg->ConfigMask = 0x7F;

	pSetCfg->DetectType = 1;//抓拍类型，0：关闭抓拍，1：离开后抓拍（默认值），2：实时抓拍，3：间隔抓拍， 4：离开后+实时抓拍。
	pSetCfg->DetectHeartbeat = 1;//心跳间隔时长，0：关闭对检测模块的心跳检测，正值：心跳检测间隔（默认为10秒）
	pSetCfg->DetectThreshold = 1;//抓拍阈值，大于抓拍阈值的人脸才会考虑。范围0-100.默认值 1; 0表示检测到的头肩结果也考虑
	pSetCfg->BackgroundCtrl = 1;//背景图上传控制;高4位控制人脸框叠加，低4位控制背景压缩率，0x00：不上传背景图，0x01：压缩率25%的背景图;0x02：压缩率60%的背景图，0x04：压缩率90%的背景图;0x00（高4位0）：背景图不叠加人脸;0x10：背景图叠加人脸框
	pSetCfg->FaceCtrl = 0x11;//人脸图控制;（高4位控制人脸图类型，低4位控制人脸图压缩率）0x10：人脸图;0x20 头肩照;0x40 半身照; 0x80 全身照;0x01压缩率25&的人脸图，0x02压缩率60%的人脸图;0x04压缩率90%的人脸图
	pSetCfg->DetectTime = 1;////在实时抓拍，表示人脸出现X秒后抓拍；间隔抓拍中，表示每隔X秒抓拍一张。
	pSetCfg->FaceSize = 1; //最小人脸;小于最小人脸的数据会别忽略。取值0-512. 默认值0

	setDetectConfig(m_TFHandle, pSetCfg);
}


int ST_11_setDetectConfig::run()
{
	if(NULL == m_TFHandle)
	{
		m_TFHandle = (void *)1;
	}
	int ret32 = 0;
	
	DetectCfg pCfg;
	DetectCfg setCfg;
	setCfg.ConfigMask = 0x7F;
	setCfg.BackgroundCtrl = 2;
	setCfg.DetectHeartbeat= 2;
	setCfg.DetectThreshold= 2;
	setCfg.DetectTime = 2;
	setCfg.DetectType = 2;
	setCfg.FaceCtrl = 2;
	setCfg.FaceSize = 2;
	DetectCfg getCfg;

	printf("ST_11_setDetectConfig_1 \n");
	ret32 = setDetectConfig(NULL, NULL);
	if(ret32 < 0)
	{
		printf("ST_11_setDetectConfig_1  ok\n\n");
	}else{
		printf("ST_11_setDetectConfig_1  ERR\n\n");
	}

	printf("ST_11_setDetectConfig_2 \n");
	ret32 = setDetectConfig(NULL, &pCfg);
	if(ret32 < 0)
	{
		printf("ST_11_setDetectConfig_2  ok\n\n");
	}else{
		printf("ST_11_setDetectConfig_2  ERR\n\n");
	}



	printf("ST_11_setDetectConfig_3 \n");
	initSetCfg(&setCfg);
	setCfg.ConfigMask = 0x01;
	setCfg.DetectType = 0x02;
	setDetectConfig(m_TFHandle, &setCfg);
	getCfg.ConfigMask = 0x01;
	ret32 = getDetectConfig(m_TFHandle, &getCfg);
	if( (ret32 == 0)&&(2 == getCfg.DetectType))
	{
		printf("ST_11_setDetectConfig_3  ok\n\n");
	}else{
		printf("ST_11_setDetectConfig_3  ERR\n\n");
	}


	printf("ST_11_setDetectConfig_4 \n");
	initSetCfg(&setCfg);
	setCfg.ConfigMask = 0x01;
	setCfg.DetectType = 0x00;
	setDetectConfig(m_TFHandle, &setCfg);
	getCfg.ConfigMask = 0x01;
	ret32 = getDetectConfig(m_TFHandle, &getCfg);
	if( (ret32 == 0)&&(0 == getCfg.DetectType))
	{
		printf("ST_11_setDetectConfig_4  ok\n\n");
	}else{
		printf("ST_11_setDetectConfig_4  ERR\n\n");
	}



	printf("ST_11_setDetectConfig_5 \n");
	initSetCfg(&setCfg);
	setCfg.ConfigMask = 0x01;
	setCfg.DetectType = 0x04;
	setDetectConfig(m_TFHandle, &setCfg);
	getCfg.ConfigMask = 0x01;
	ret32 = getDetectConfig(m_TFHandle, &getCfg);
	printf("get getCfg.DetectType = %d\n",getCfg.DetectType);
	if( (ret32 == 0)&&(4 == getCfg.DetectType))
	{
		printf("ST_11_setDetectConfig_5  ok\n\n");
	}else{
		printf("ST_11_setDetectConfig_5  ERR\n\n");
	}




	printf("ST_11_setDetectConfig_6 \n");
	initSetCfg(&setCfg);
	setCfg.ConfigMask = 0x01;
	setCfg.DetectType = 0x09;
	ret32 = setDetectConfig(m_TFHandle, &setCfg);
	getCfg.ConfigMask = 0x01;
	ret32 |= getDetectConfig(m_TFHandle, &getCfg);
	if( ret32 < 0)
	{
		printf("ST_11_setDetectConfig_6  ok\n\n");
	}else{
		printf("ST_11_setDetectConfig_6  ERR\n\n");
	}




	printf("ST_11_setDetectConfig_7 \n");
	initSetCfg(&setCfg);
	setCfg.ConfigMask = 0x02;
	setCfg.DetectHeartbeat = 0x10;
	ret32 = setDetectConfig(m_TFHandle, &setCfg);
	getCfg.ConfigMask = 0x02;
	ret32 |= getDetectConfig(m_TFHandle, &getCfg);
	//printf("ret32 = %d,getCfg.DetectHeartbeat = %x\n",ret32,getCfg.DetectHeartbeat);
	if( (ret32 == 0)&&(0x10 == getCfg.DetectHeartbeat) )
	{
		printf("ST_11_setDetectConfig_7  ok\n\n");
	}else{
		printf("ST_11_setDetectConfig_7  ERR\n\n");
	}

	
	printf("ST_11_setDetectConfig_8 \n");
	initSetCfg(&setCfg);
	setCfg.ConfigMask = 0x02;
	setCfg.DetectHeartbeat = 0;
	ret32 = setDetectConfig(m_TFHandle, &setCfg);
	getCfg.ConfigMask = 0x02;
	ret32 |= getDetectConfig(m_TFHandle, &getCfg);
	//printf("ret32 = %d,getCfg.DetectHeartbeat = %x\n",ret32,getCfg.DetectHeartbeat);
	if( (ret32 == 0)&&(0x0 == getCfg.DetectHeartbeat) )
	{
		printf("ST_11_setDetectConfig_8  ok\n\n");
	}else{
		printf("ST_11_setDetectConfig_8  ERR\n\n");
	}


	printf("ST_11_setDetectConfig_9 \n");
	initSetCfg(&setCfg);
	setCfg.ConfigMask = 0x02;
	setCfg.DetectHeartbeat = 255;
	ret32 = setDetectConfig(m_TFHandle, &setCfg);
	getCfg.ConfigMask = 0x02;
	ret32 |= getDetectConfig(m_TFHandle, &getCfg);
	//printf("ret32 = %d,getCfg.DetectHeartbeat = %x\n",ret32,getCfg.DetectHeartbeat);
	if(( 0 == ret32 ) &&(255 == getCfg.DetectHeartbeat ))
	{
		printf("ST_11_setDetectConfig_9  ok\n\n");
	}else{
		printf("ST_11_setDetectConfig_9  ERR\n\n");
	}

	printf("ST_11_setDetectConfig_10 \n");
	initSetCfg(&setCfg);
	setCfg.ConfigMask = 0x04;
	setCfg.DetectThreshold = 10;
	ret32 = setDetectConfig(m_TFHandle, &setCfg);
	getCfg.ConfigMask = setCfg.ConfigMask;
	ret32 |= getDetectConfig(m_TFHandle, &getCfg);
	//printf("ret32 = %d,getCfg.DetectHeartbeat = %x\n",ret32,getCfg.DetectHeartbeat);
	if(( ret32 == 0 ) &&(getCfg.DetectThreshold == 10))
	{
		printf("ST_11_setDetectConfig_10  ok\n\n");
	}else{
		printf("ST_11_setDetectConfig_10  ERR\n\n");
	}


	printf("ST_11_setDetectConfig_11 \n");
	initSetCfg(&setCfg);
	setCfg.ConfigMask = 0x04;
	setCfg.DetectThreshold = 100;
	ret32 = setDetectConfig(m_TFHandle, &setCfg);
	getCfg.ConfigMask = setCfg.ConfigMask;
	ret32 |= getDetectConfig(m_TFHandle, &getCfg);
	//printf("ret32 = %d,getCfg.DetectHeartbeat = %x\n",ret32,getCfg.DetectHeartbeat);
	if( ( ret32 ==  0 ) &&(100 == getCfg.DetectThreshold))
	{
		printf("ST_11_setDetectConfig_11  ok\n\n");
	}else{
		printf("ST_11_setDetectConfig_11  ERR\n\n");
	}


	printf("ST_11_setDetectConfig_12 \n");
	initSetCfg(&setCfg);
	setCfg.ConfigMask = 0x04;
	setCfg.DetectThreshold = 155;
	ret32 = setDetectConfig(m_TFHandle, &setCfg);
	getCfg.ConfigMask = setCfg.ConfigMask;
	ret32 |= getDetectConfig(m_TFHandle, &getCfg);
	//printf("ret32 = %d,getCfg.DetectHeartbeat = %x\n",ret32,getCfg.DetectHeartbeat);
	if( ( ret32 < 0))
	{
		printf("ST_11_setDetectConfig_12  ok\n\n");
	}else{
		printf("ST_11_setDetectConfig_12  ERR\n\n");
	}

	printf("ST_11_setDetectConfig_13 \n");
	initSetCfg(&setCfg);
	setCfg.ConfigMask = 0x08;
	setCfg.BackgroundCtrl = 0x12;
	ret32 = setDetectConfig(m_TFHandle, &setCfg);
	getCfg.ConfigMask = setCfg.ConfigMask;
	ret32 |= getDetectConfig(m_TFHandle, &getCfg);
	//printf("ret32 = %d,getCfg.DetectHeartbeat = %x\n",ret32,getCfg.DetectHeartbeat);
	if( ( 0 == ret32 ) &&(0x12 == getCfg.BackgroundCtrl))
	{
		printf("ST_11_setDetectConfig_13  ok\n\n");
	}else{
		printf("ST_11_setDetectConfig_13  ERR\n\n");
	}

	printf("ST_11_setDetectConfig_14 \n");
	initSetCfg(&setCfg);
	setCfg.ConfigMask = 0x08;
	setCfg.BackgroundCtrl = 0x14;
	ret32 = setDetectConfig(m_TFHandle, &setCfg);
	getCfg.ConfigMask = setCfg.ConfigMask;
	ret32 |= getDetectConfig(m_TFHandle, &getCfg);
	//printf("ret32 = %d,getCfg.DetectHeartbeat = %x\n",ret32,getCfg.DetectHeartbeat);
	if( ( 0 == ret32 ) &&(0x14 == getCfg.BackgroundCtrl) )
	{
		printf("ST_11_setDetectConfig_14  ok\n\n");
	}else{
		printf("ST_11_setDetectConfig_14  ERR\n\n");
	}

	printf("ST_11_setDetectConfig_15 \n");
	initSetCfg(&setCfg);
	setCfg.ConfigMask = 0x08;
	setCfg.BackgroundCtrl = 0x15;
	ret32 = setDetectConfig(m_TFHandle, &setCfg);
	getCfg.ConfigMask = setCfg.ConfigMask;
	ret32 |= getDetectConfig(m_TFHandle, &getCfg);
	//printf("ret32 = %d,getCfg.DetectHeartbeat = %x\n",ret32,getCfg.DetectHeartbeat);
	if( ret32 < 0)
	{
		printf("ST_11_setDetectConfig_15  ok\n\n");
	}else{
		printf("ST_11_setDetectConfig_15  ERR\n\n");
	}

	printf("ST_11_setDetectConfig_16 \n");
	initSetCfg(&setCfg);
	setCfg.ConfigMask = 0x08;
	setCfg.BackgroundCtrl = 0x25;
	ret32 = setDetectConfig(m_TFHandle, &setCfg);
	getCfg.ConfigMask = setCfg.ConfigMask;
	ret32 |= getDetectConfig(m_TFHandle, &getCfg);
	//printf("ret32 = %d,getCfg.DetectHeartbeat = %x\n",ret32,getCfg.DetectHeartbeat);
	if(ret32 < 0)
	{
		printf("ST_11_setDetectConfig_16  ok\n\n");
	}else{
		printf("ST_11_setDetectConfig_16  ERR\n\n");
	}


	printf("ST_11_setDetectConfig_17 \n");
	initSetCfg(&setCfg);
	setCfg.ConfigMask = 0x10;
	setCfg.FaceCtrl = 0x12;
	ret32 = setDetectConfig(m_TFHandle, &setCfg);
	getCfg.ConfigMask = setCfg.ConfigMask;
	ret32 |= getDetectConfig(m_TFHandle, &getCfg);
	//printf("ret32 = %d,getCfg.DetectHeartbeat = %x\n",ret32,getCfg.DetectHeartbeat);
	if( ( 0 == ret32 ) &&(0x12 == getCfg.FaceCtrl) )
	{
		printf("ST_11_setDetectConfig_17  ok\n\n");
	}else{
		printf("ST_11_setDetectConfig_17  ERR\n\n");
	}


	printf("ST_11_setDetectConfig_18 \n");
	initSetCfg(&setCfg);
	setCfg.ConfigMask = 0x10;
	setCfg.FaceCtrl = 0x84;
	ret32 = setDetectConfig(m_TFHandle, &setCfg);
	getCfg.ConfigMask = setCfg.ConfigMask;
	ret32 |= getDetectConfig(m_TFHandle, &getCfg);
	//printf("ret32 = %d,getCfg.DetectHeartbeat = %x\n",ret32,getCfg.DetectHeartbeat);
	if( ( 0 == ret32 ) &&(0x84 == getCfg.FaceCtrl) )
	{
		printf("ST_11_setDetectConfig_18  ok\n\n");
	}else{
		printf("ST_11_setDetectConfig_18  ERR\n\n");
	}


	printf("ST_11_setDetectConfig_19 \n");
	initSetCfg(&setCfg);
	setCfg.ConfigMask = 0x10;
	setCfg.FaceCtrl = 0;
	ret32 = setDetectConfig(m_TFHandle, &setCfg);
	getCfg.ConfigMask = setCfg.ConfigMask;
	ret32 |= getDetectConfig(m_TFHandle, &getCfg);
	//printf("ret32 = %d,getCfg.DetectHeartbeat = %x\n",ret32,getCfg.DetectHeartbeat);
	if(  ret32 < 0 )
	{
		printf("ST_11_setDetectConfig_19  ok\n\n");
	}else{
		printf("ST_11_setDetectConfig_19  ERR\n\n");
	}





	printf("ST_11_setDetectConfig_20 \n");
	initSetCfg(&setCfg);
	setCfg.ConfigMask = 0x10;
	setCfg.FaceCtrl = 0x08;
	ret32 = setDetectConfig(m_TFHandle, &setCfg);
	getCfg.ConfigMask = setCfg.ConfigMask;
	ret32 |= getDetectConfig(m_TFHandle, &getCfg);
	//printf("ret32 = %d,getCfg.DetectHeartbeat = %x\n",ret32,getCfg.DetectHeartbeat);
	if(  ret32 < 0 )
	{
		printf("ST_11_setDetectConfig_20  ok\n\n");
	}else{
		printf("ST_11_setDetectConfig_20  ERR\n\n");
	}

	
	printf("ST_11_setDetectConfig_21 \n");
	initSetCfg(&setCfg);
	setCfg.ConfigMask = 0x20;
	setCfg.DetectTime = 0x12;
	ret32 = setDetectConfig(m_TFHandle, &setCfg);
	getCfg.ConfigMask = setCfg.ConfigMask;
	ret32 |= getDetectConfig(m_TFHandle, &getCfg);
	//printf("ret32 = %d,getCfg.DetectHeartbeat = %x\n",ret32,getCfg.DetectHeartbeat);
	if(  (0 == ret32) &&  (0x12 == getCfg.DetectTime)  )
	{
		printf("ST_11_setDetectConfig_21  ok\n\n");
	}else{
		printf("ST_11_setDetectConfig_21  ERR\n\n");
	}


	printf("ST_11_setDetectConfig_22 \n");
	initSetCfg(&setCfg);
	setCfg.ConfigMask = 0x20;
	setCfg.DetectTime = 0;
	ret32 = setDetectConfig(m_TFHandle, &setCfg);
	getCfg.ConfigMask = setCfg.ConfigMask;
	ret32 |= getDetectConfig(m_TFHandle, &getCfg);
	//printf("ret32 = %d,getCfg.DetectHeartbeat = %x\n",ret32,getCfg.DetectHeartbeat);
	if(  (0 == ret32) &&  (0 == getCfg.DetectTime)  )
	{
		printf("ST_11_setDetectConfig_22  ok\n\n");
	}else{
		printf("ST_11_setDetectConfig_22  ERR\n\n");
	}


	
	printf("ST_11_setDetectConfig_23 \n");
	initSetCfg(&setCfg);
	setCfg.ConfigMask = 0x40;
	setCfg.FaceSize = 0x12;
	ret32 = setDetectConfig(m_TFHandle, &setCfg);
	getCfg.ConfigMask = setCfg.ConfigMask;
	ret32 |= getDetectConfig(m_TFHandle, &getCfg);
	//printf("ret32 = %d,getCfg.DetectHeartbeat = %x\n",ret32,getCfg.DetectHeartbeat);
	if(  (0 == ret32) &&  (0x12 == getCfg.FaceSize)  )
	{
		printf("ST_11_setDetectConfig_23  ok\n\n");
	}else{
		printf("ST_11_setDetectConfig_23  ERR\n\n");
	}

	printf("ST_11_setDetectConfig_24 \n");
	initSetCfg(&setCfg);
	setCfg.ConfigMask = 0x40;
	setCfg.FaceSize = 0;
	ret32 = setDetectConfig(m_TFHandle, &setCfg);
	getCfg.ConfigMask = setCfg.ConfigMask;
	ret32 |= getDetectConfig(m_TFHandle, &getCfg);
	//printf("ret32 = %d,getCfg.DetectHeartbeat = %x\n",ret32,getCfg.DetectHeartbeat);
	if(  (0 == ret32) &&  (0 == getCfg.FaceSize)  )
	{
		printf("ST_11_setDetectConfig_24  ok\n\n");
	}else{
		printf("ST_11_setDetectConfig_24  ERR\n\n");
	}

	printf("ST_11_setDetectConfig_25 \n");
	initSetCfg(&setCfg);
	setCfg.ConfigMask = 0x40;
	setCfg.FaceSize = 513;
	ret32 = setDetectConfig(m_TFHandle, &setCfg);
	getCfg.ConfigMask = setCfg.ConfigMask;
	ret32 |= getDetectConfig(m_TFHandle, &getCfg);
	//printf("ret32 = %d,getCfg.DetectHeartbeat = %x\n",ret32,getCfg.DetectHeartbeat);
	if( ret32 < 0 )
	{
		printf("ST_11_setDetectConfig_25  ok\n\n");
	}else{
		printf("ST_11_setDetectConfig_25  ERR\n\n");
	}

	printf("ST_11_setDetectConfig_26 \n");
	initSetCfg(&setCfg);
	ret32 = setDetectConfig(m_TFHandle, &setCfg);
	getCfg.ConfigMask = 0x7F;
	ret32 |= getDetectConfig(m_TFHandle, &getCfg);
	//printf("ret32 = %d,getCfg.DetectHeartbeat = %x\n",ret32,getCfg.DetectHeartbeat);
	if(  (0 == ret32) &&  (1 == getCfg.DetectTime)&&  (1 == getCfg.DetectHeartbeat)&&  (1 == getCfg.DetectThreshold)
						&&  (1 == getCfg.DetectType)&&  (1 == getCfg.BackgroundCtrl)
						&&  (0x11 == getCfg.FaceCtrl)&&  (1 == getCfg.FaceSize))
	{
		printf("ST_11_setDetectConfig_26  ok\n\n");
	}else{
		printf("ST_11_setDetectConfig_26  ERR\n\n");
	}

	printf("-------test getDetectConfig ok -----\n");

	return 0;
}


int ST_11_setDetectConfig::uninit()
{
	Driver::uninit();
}

ST_11_setDetectConfig::~ST_11_setDetectConfig()
{
	
}



void ST_11_setDetectConfig::usrCallbackReplyCancalClient1(UINT64 *pReqID,
											  UINT8 *pRet,
											  UINT8 *pReplyData,
											  UINT32 *pDataLen )
{
	printf("---the requestion(%lld) CANCAL CLIENT replied: ret=%d, datalen=%d",
		   *pReqID, *pRet, *pDataLen );
	
}


void ST_11_setDetectConfig::usrCallbackReplyRegisterClient1(UINT64 *pReqID,
												UINT8 *pRet,
												UINT8 *pReplyData,
												UINT32 *pDataLen )
{
	printf("---the requestion(%lld) REGISTER CLIENT replied: ret=%d, datalen=%d",
		   *pReqID, *pRet, *pDataLen );
  
}

void ST_11_setDetectConfig::usrCallbackNotifyServiceInfo1( RWNotifyType *pNotifyType,
											   UINT8 *pExtData,
											   UINT32 *pDataLen)
{
	//printf("---the notify CLIENT INFO type=%d, datalen=%d", *pNotifyType, *pDataLen );
}
void ST_11_setDetectConfig::usrCallbackNotifyFaceInfo1( RWNotifyType *pNotifyType,
											UINT8 *pExtData,
											UINT32 *pDataLen )
{
	printf("---the notify IMG INFO type=%d, datalen=%d",
		   *pNotifyType, *pDataLen );
	
}

	 
	

