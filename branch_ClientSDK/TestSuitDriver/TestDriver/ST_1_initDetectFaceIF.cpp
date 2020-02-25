/********************************************************************
* Name      :  ST_1_initDetectFaceIF.cpp                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  Liu Xin                                          
* Date      :  2018-8-22                                                
********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "DetectFaceIF.h"
#include "ST_1_initDetectFaceIF.h"
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




ST_1_initDetectFaceIF::ST_1_initDetectFaceIF(unsigned int nCaseId ): Driver( nCaseId )
{
	
}

int ST_1_initDetectFaceIF::init_pConfig(DetectConfigInfo **pCfgInfo)
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
	*pCfgInfo = &cfgInfo;
	//printf("pCfgInfo = %x\n",*pCfgInfo);
		
	return 0;

}

int ST_1_initDetectFaceIF::init()
{
	int ret32 = 0;
	void *TFHandle;
	TFHandle = &ret32;
	DetectConfigInfo * pConfig;

	printf("ST_1_initDetectFaceIF_1 \n");
	TFHandle = NULL;
	pConfig  = NULL;
	ret32 = initDetectFaceIF( &TFHandle, pConfig );
	if(ret32 < 0)
	{
		printf("ST_1_initDetectFaceIF_1 OK \n\n");
	}else{
		printf("ST_1_initDetectFaceIF_1 ERR \n\n");
	}


	printf("ST_1_initDetectFaceIF_2 \n");
	TFHandle = NULL;
	init_pConfig(&pConfig);
	ret32 = initDetectFaceIF( &TFHandle, pConfig );
	if(ret32 < 0)
	{
		printf("ST_1_initDetectFaceIF_2 OK \n\n");
	}else{
		printf("ST_1_initDetectFaceIF_2 ERR \n\n");
	}

	printf("ST_1_initDetectFaceIF_3 \n");
	TFHandle = (void*)1;
	pConfig  = NULL;
	ret32 = initDetectFaceIF( &TFHandle, pConfig );
	if(ret32 < 0 )
	{
		printf("ST_1_initDetectFaceIF_3 OK \n \n");
	}else{
		printf("ST_1_initDetectFaceIF_3 ERR \n\n");
	}

	printf("ST_1_initDetectFaceIF_4 \n");
	TFHandle = (void*)1;
	init_pConfig(&pConfig);
	//printf("pCfgInfo = %x,cfgInfo = %x\n",pConfig,&cfgInfo);
	pConfig->DetectType = 5;
	//printf("pConfig->DetectType = %d\n",pConfig->DetectType);
	ret32 = initDetectFaceIF( &TFHandle, pConfig );
	if(ret32 < 0)
	{
		printf("ST_1_initDetectFaceIF_4  OK \n\n");
	}else{
		printf("ST_1_initDetectFaceIF_4 ERR \n\n");
	}
	

	printf("ST_1_initDetectFaceIF_5 \n");
	TFHandle = (void*)1;
	init_pConfig(&pConfig);
	pConfig->SerialPortInfoPtr = NULL;
	pConfig->DetectType = 0x00;
	//printf("pConfig->DetectType = %d\n",pConfig->DetectType);
	//printf("pConfig->FrameInfo = %x,width = %d, height = %d \n",pConfig->FrameInfo, pConfig->FrameInfo->Width, pConfig->FrameInfo->Height);
	ret32 = initDetectFaceIF( &TFHandle, pConfig );
	if(0 == ret32)
	{
		printf("ST_1_initDetectFaceIF_5  OK \n\n");
	}else{
		printf("ST_1_initDetectFaceIF_5 ERR \n\n");
	}

	printf("ST_1_initDetectFaceIF_6 \n");
	TFHandle = (void*)1;
	init_pConfig(&pConfig);
	pConfig->DetectType = 0x01;
	ret32 = initDetectFaceIF( &TFHandle, pConfig );
	if(0 == ret32)
	{
		printf("ST_1_initDetectFaceIF_6  OK \n\n");
	}else{
		printf("ST_1_initDetectFaceIF_6 ERR \n\n");
	}

	printf("ST_1_initDetectFaceIF_7 \n");
	TFHandle = (void*)1;
	init_pConfig(&pConfig);
	pConfig->TransferType = 0x09;
	ret32 = initDetectFaceIF( &TFHandle, pConfig );
	if(ret32 < 0)
	{
		printf("ST_1_initDetectFaceIF_7  OK \n\n");
	}else{
		printf("ST_1_initDetectFaceIF_7 ERR \n\n");
	}
	

	printf("ST_1_initDetectFaceIF_8 \n");
	TFHandle = (void*)1;
	init_pConfig(&pConfig);
	pConfig->TransferType = 0;
	ret32 = initDetectFaceIF( &TFHandle, pConfig );
	if(0 == ret32)
	{
		printf("ST_1_initDetectFaceIF_8  OK \n\n");
	}else{
		printf("ST_1_initDetectFaceIF_8 ERR \n\n");
	}

	printf("ST_1_initDetectFaceIF_9\n");
	TFHandle = (void*)1;
	init_pConfig(&pConfig);
	pConfig->TransferType = 1;
	ret32 = initDetectFaceIF( &TFHandle, pConfig);
	if(0 ==ret32)
	{
		printf("ST_1_initDetectFaceIF_9 OK \n!\n");
	}else{
		printf("ST_1_initDetectFaceIF_9 ERR \n\n");
	}

	printf("ST_1_initDetectIF_10 \n");
	TFHandle = (void*)1;
	init_pConfig(&pConfig);
	pConfig->TransferType = 2;
	ret32 = initDetectFaceIF(&TFHandle, pConfig);
	if(0 == ret32)
	{
		printf("ST_1_initDetectIF_10  OK \n\n");
	}else{
		printf("ST_1_initDetectFaceIF_10 ERR \n\n");
	}



	printf("ST_1_initDetect_IF_11 \n");
	TFHandle= (void*)1;
	init_pConfig(&pConfig);
	pConfig->BuffCount = 10;
	ret32 = initDetectFaceIF(&TFHandle, pConfig);
	if(0 == ret32)
	{
		printf("ST_1_initDetectIF_11 OK \n \n");
	}else{
		printf("ST_1_initDetectFaceIF_11 ERR \n\n");
	}

	printf("ST_1_initDetectIF_12 \n");
	TFHandle = (void*)1;
	init_pConfig(&pConfig);
	pConfig->CameraID = 123;
	ret32 = initDetectFaceIF(&TFHandle, pConfig);
	if(0 == ret32)
	{
		printf("ST_1_initDetect_IF_12 OK \n\n");
	}else{
		printf("ST_1_initDetectFaceIF_12 ERR \n\n");
	}


	printf("ST_1_initDetectIF_13 \n");
	TFHandle = (void *)1;
	init_pConfig(&pConfig);
	pConfig->BuffSize = 123;
	ret32 = initDetectFaceIF(&TFHandle, pConfig);
	if(0 ==ret32)
	{
		printf("ST_1_initDetect_IF13 OK \n\n");
	}else{
		printf("ST_1_initDetectFaceIF_13 ERR \n\n");
	}

	printf("ST_1_initDetectIF_14 \n");
	TFHandle = (void*)1;
	init_pConfig(&pConfig);
	pConfig->SerialPortInfoPtr = NULL;
	ret32 = initDetectFaceIF(&TFHandle, pConfig);
	if(ret32 < 0 )
	{
		printf("ST_1_initDetect_IF_14 OK \n \n");
	}else{
		printf("ST_1_initDetectFaceIF_14 ERR \n\n");
	}

	printf("ST_1_initDetect_15 \n");
	init_pConfig(&pConfig);
	pConfig->SerialPortInfoPtr->DevPathPtr = NULL;
	ret32 = initDetectFaceIF(&TFHandle, pConfig);
	if(ret32 <0 )
	{
		printf("ST_1_1_initDetect_IF_15 OK \n \n");
	}else{
		printf("ST_1_initDetectFaceIF_15 ERR \n\n");
	}


	printf("ST_1_initDetect_IF_16 \n");
	TFHandle = (void*)1;
	init_pConfig(&pConfig);
	pConfig->DetectType = 0;
	ret32 = initDetectFaceIF(&TFHandle, pConfig);
	if(ret32 == 0 )
	{
		printf("ST_1_initDetect_IF 16 OK \n\n");
	}else{
		printf("ST_1_initDetectFaceIF_16 ERR \n\n");
	}

	printf("ST_1_initDetect_IF_17 \n");
	TFHandle = (void*)1;
	init_pConfig(&pConfig);
	pConfig->DetectType = 1;
	ret32 = initDetectFaceIF(&TFHandle, pConfig);
	if(ret32 == 0 )
	{
		printf("ST_1_initDetect_IF 17 OK \n\n");
	}else{
		printf("ST_1_initDetectFaceIF_17 ERR \n\n");
	}

	printf("ST_1_initDetect_IF_18 \n");
	TFHandle = (void *) 1;
	init_pConfig(&pConfig);
	pConfig->SerialPortInfoPtr->PathLen = 0;
	ret32 = initDetectFaceIF(&TFHandle, pConfig);
	if(ret32 <0)
	{
		printf("ST_1_initDetect_IF 18 OK \n\n");
	}else{
		printf("ST_1_initDetectFaceIF_18 ERR \n\n");
	}
	

	printf("ST_1_initDetect_IF 19\n");
	TFHandle = (void*)1;
	init_pConfig(&pConfig);
	pConfig->SerialPortInfoPtr->Baudrate = 0;
	ret32 = initDetectFaceIF(&TFHandle, pConfig);
	if(ret32 < 0)
	{
		printf("ST_1_initDetect_IF 19 OK \n\n");
	}else{
		printf("ST_1_initDetectFaceIF_19 ERR \n\n");
	}

	printf("ST_1_initDetect_IF_20 \n");
	TFHandle = (void *) 1;
	init_pConfig(&pConfig);
	pConfig->SerialPortInfoPtr->Baudrate = 115200;
	ret32 = initDetectFaceIF(&TFHandle, pConfig);
	if(0 == ret32)
	{
		printf("ST_1_initDetect_IF_20 OK \n\n");
	}else{
		printf("ST_1_initDetectFaceIF_20 ERR \n\n");
	}

	printf("ST_1_initDetect_IF_21 \n");
	TFHandle = (void *)1;
	init_pConfig(&pConfig);
	pConfig->SrcUserInfo = NULL;
	ret32 = initDetectFaceIF(&TFHandle, pConfig);
	if(ret32 < 0)
	{
		printf("ST_1_initDetectFaceIF_IF 21 OK \n\n");
	}else{
		printf("ST_1_initDetectFaceIF_21 ERR \n\n");
	}

	printf("ST_1_initDetectFaceIF_22 \n");
	TFHandle = (void *)1;
	init_pConfig(&pConfig);
	pConfig->DetectType = 0;
	ret32 = initDetectFaceIF(&TFHandle, pConfig);
	if(0 == ret32)
	{
		printf("ST_1_initDetectFaceIF_22 OK \n\n");
	}else{
		printf("ST_1_initDetectFaceIF_22 ERR \n\n");
	}
	


	printf("ST_1_initDetectFaceIF_23 \n");
	TFHandle = (void *)1;
	init_pConfig(&pConfig);
	pConfig->DetectType = 1;
	ret32 = initDetectFaceIF(&TFHandle, pConfig);
	if(0 == ret32 )
	{
		printf("ST_1_initDetectFaceIF_23 OK \n\n");
	}else{
		printf("ST_1_initDetectFaceIF_23 ERR \n\n");
	}


	printf("ST_1_initDetectFaceIF_24 \n");
	TFHandle = (void *)1;
	init_pConfig(&pConfig);
	pConfig->DetectType = 2;
	ret32 = initDetectFaceIF(&TFHandle, pConfig);
	if(ret32 < 0)
	{
		printf("ST_1_initDetectFaceIF_24 OK \n\n");
	}else{
		printf("ST_1_initDetectFaceIF_24 ERR \n\n");
	}


	
	printf("ST_1_initDetectFaceIF_25 \n");
	TFHandle = (void *)1;
	init_pConfig(&pConfig);
	pConfig->SrcUserInfo->EncryptType = 5;
	ret32 = initDetectFaceIF(&TFHandle, pConfig);
	if(ret32 < 0)
	{
		printf("ST_1_initDetectFaceIF_25 OK \n\n");
	}else{
		printf("ST_1_initDetectFaceIF_25 ERR \n\n");
	}


	printf("ST_1_initDetectFaceIF_26 \n");
	TFHandle = (void *)1;
	init_pConfig(&pConfig);
	pConfig->SrcUserInfo->EncryptType = 0;
	ret32 = initDetectFaceIF(&TFHandle, pConfig);
	if( 0 == ret32)
	{
		printf("ST_1_initDetectFaceIF_26 OK \n\n");
	}else{
		printf("ST_1_initDetectFaceIF_26 ERR \n\n");
	}



	printf("ST_1_initDetectFaceIF_27 \n");
	TFHandle = (void *)1;
	init_pConfig(&pConfig);
	pConfig->SrcUserInfo->EncryptType = 1;
	ret32 = initDetectFaceIF(&TFHandle, pConfig);
	if( 0 == ret32)
	{
		printf("ST_1_initDetectFaceIF_27 OK \n\n");
	}else{
		printf("ST_1_initDetectFaceIF_27 ERR \n\n");
	}

	printf("ST_1_initDetectFaceIF_28 \n");
	TFHandle = (void *)1;
	init_pConfig(&pConfig);
	pConfig->SrcUserInfo->Authorization = 6;
	ret32 = initDetectFaceIF(&TFHandle, pConfig);
	if(0 == ret32)
	{
		printf("ST_1_initDetectFaceIF_28 OK \n\n");
	}else{
		printf("ST_1_initDetectFaceIF_28 ERR \n\n");
	}

	printf("ST_1_initDetectFaceIF_29 \n");
	TFHandle = (void *)1;
	init_pConfig(&pConfig);
	char myUserNameT2[UserNameLen]="\0";
	for(int i = 0; i < UserNameLen; i++ )
	{
		myUserNameT2[i] = 'A';
	}
	memcpy( (void*)pConfig->SrcUserInfo->UserName, myUserNameT2, UserNameLen );
	ret32 = initDetectFaceIF(&TFHandle, pConfig);
	if(0 == ret32)
	{
		printf("ST_1_initDetectFaceIF_29 OK \n\n");
	}else{
		printf("ST_1_initDetectFaceIF_29 ERR \n\n");
	}


	printf("ST_1_initDetectFaceIF_30 \n");
	TFHandle = (void *)1;
	init_pConfig(&pConfig);
	UINT8 myUserName[UserNameLen] = "lx";
	memcpy(&(pConfig->SrcUserInfo->UserName),&myUserName,UserNameLen);
	ret32 = initDetectFaceIF(&TFHandle, pConfig);
	if(0 == ret32)
	{
		printf("ST_1_initDetectFaceIF_30 OK \n\n");
	}else{
		printf("ST_1_initDetectFaceIF_30 ERR \n\n");
	}


	printf("ST_1_initDetectFaceIF_31 \n");
	TFHandle = (void *)1;
	init_pConfig(&pConfig);
	pConfig->SrcUserInfo->UserPWD;
	ret32 = initDetectFaceIF(&TFHandle, pConfig);
	if(0 == ret32)
	{
		printf("ST_1_initDetectFaceIF_31 OK \n \n");
	}else{
		printf("ST_1_initDetectFaceIF_31 ERR \n\n");
	}



	printf("ST_1_initDetectFaceIF_32 \n");
	TFHandle = (void *)1;
	init_pConfig(&pConfig);
	UINT8 myUserPWD[UserPWDLen] = "123456";
	memcpy(&(pConfig->SrcUserInfo->UserPWD),&myUserPWD,UserPWDLen);
	ret32 = initDetectFaceIF(&TFHandle, pConfig);
	if(0 == ret32)
	{
		printf("ST_1_initDetectFaceIF_32 OK \n \n");
	}else{
		printf("ST_1_initDetectFaceIF_32 ERR \n\n");
	}




	printf("ST_1_initDetectFaceIF_33 \n");
	TFHandle = (void *)1;
	init_pConfig(&pConfig);
	pConfig->SrcIFInfo = NULL;
	ret32 = initDetectFaceIF(&TFHandle, pConfig);
	if(ret32 < 0)
	{
		printf("ST_1_initDetectFaceIF_33 OK \n \n");
	}else{
		printf("ST_1_initDetectFaceIF_33 ERR \n\n");
	}

	
	printf("ST_1_initDetectFaceIF_34 \n");
	TFHandle = (void *)1;
	init_pConfig(&pConfig);
	pConfig->SrcIFInfo->IPType = 5;
	ret32 = initDetectFaceIF(&TFHandle, pConfig);
	if(ret32 < 0)
	{
		printf("ST_1_initDetectFaceIF_34 OK \n\n");
	}else{
		printf("ST_1_initDetectFaceIF_34 ERR \n\n");
	}


	printf("ST_1_initDetectFaceIF_35 \n");
	TFHandle = (void *)1;
	init_pConfig(&pConfig);
	pConfig->SrcIFInfo->IPType = 0;
	ret32 = initDetectFaceIF(&TFHandle, pConfig);
	if(0 == ret32)
	{
		printf("ST_1_initDetectFaceIF_35 OK \n\n");
	}else{
		printf("ST_1_initDetectFaceIF_35 ERR \n\n");
	}


	printf("ST_1_initDetectFaceIF_36 \n");
	TFHandle = (void *)1;
	init_pConfig(&pConfig);
	pConfig->SrcIFInfo->IPType = 1;
	ret32 = initDetectFaceIF(&TFHandle, pConfig);
	if(0 == ret32)
	{
		printf("ST_1_initDetectFaceIF_36 OK \n\n");
	}else{
		printf("ST_1_initDetectFaceIF_36 ERR \n\n");
	}


	printf("ST_1_initDetectFaceIF_37 \n");
	TFHandle = (void *)1;
	init_pConfig(&pConfig);
	UINT8 myIPArray[16]= "1234"; 
	memcpy( &(pConfig->SrcIFInfo->IPArray), &myIPArray, 16);
	ret32 = initDetectFaceIF(&TFHandle, pConfig);
	if(0 == ret32)
	{
		printf("ST_1_initDetectFaceIF_37 OK \n \n");
	}else{
		printf("ST_1_initDetectFaceIF_37 ERR \n\n");
	}

	

	printf("ST_1_initDetectFaceIF_38 \n");
	TFHandle = (void *)1;
	init_pConfig(&pConfig);
	pConfig->SrcIFInfo->CtrlPort = 233;
	ret32 = initDetectFaceIF(&TFHandle, pConfig);
	if(0 == ret32)
	{
		printf("ST_1_initDetectFaceIF_38 OK \n \n");
	}else{
		printf("ST_1_initDetectFaceIF_38 ERR \n\n");
	}

	printf("ST_1_initDetectFaceIF_39 \n");
	TFHandle = (void *)1;
	init_pConfig(&pConfig);
	pConfig->SrcIFInfo->DataPort = 211;
	ret32 = initDetectFaceIF(&TFHandle, pConfig);
	if(0 == ret32)
	{
		printf("ST_1_initDetectFaceIF_39  OK \n\n");
	}else{
		printf("ST_1_initDetectFaceIF_39 ERR \n\n");
	}


	printf("ST_1_initDetectFaceIF_40 \n");
	TFHandle = (void *)1;
	init_pConfig(&pConfig);
	pConfig->SrcIFInfo->CameraID = 0x87654321;
	ret32 = initDetectFaceIF(&TFHandle, pConfig);
	if(0 == ret32)
	{
		printf("ST_1_initDetectFaceIF_40 OK \n \n");
	}else{
		printf("ST_1_initDetectFaceIF_40 ERR \n\n");
	}


	printf("ST_1_initDetectFaceIF_41 \n");
	TFHandle = (void *)1;
	init_pConfig(&pConfig);
	pConfig->FrameInfo = NULL;
	ret32 = initDetectFaceIF(&TFHandle, pConfig);
	if(ret32 < 0)
	{
		printf("ST_1_initDetectFaceIF_41 OK \n \n");
	}else{
		printf("ST_1_initDetectFaceIF_41 ERR \n\n");
	}

	printf("ST_1_initDetectFaceIF_42 \n");
	TFHandle = (void *)1;
	init_pConfig(&pConfig);
	pConfig->FrameInfo->ImgID  = 12;
	ret32 = initDetectFaceIF(&TFHandle, pConfig);
	if(ret32 == 0)
	{
		printf("ST_1_initDetectFaceIF_42  OK \n \n");
	}else{
		printf("ST_1_initDetectFaceIF_42 ERR \n\n");
	}

	printf("ST_1_initDetectFaceIF_43 \n");
	TFHandle = (void *)1;
	init_pConfig(&pConfig);
	pConfig->FrameInfo->Width  = 0;
	ret32 = initDetectFaceIF(&TFHandle, pConfig);
	if(ret32 < 0)
	{
		printf("ST_1_initDetectFaceIF_43 OK \n \n");
	}else{
		printf("ST_1_initDetectFaceIF_43 ERR \n\n");
	}

	
	printf("ST_1_initDetectFaceIF_44 \n");
	TFHandle = (void *)1;
	init_pConfig(&pConfig);
	pConfig->FrameInfo->Width  = 1080;
	ret32 = initDetectFaceIF(&TFHandle, pConfig);
	if(0 == ret32 )
	{
		printf("ST_1_initDetectFaceIF_44 OK \n\n");
	}else{
		printf("ST_1_initDetectFaceIF_44 ERR \n\n");
	}


	printf("ST_1_initDetectFaceIF_45 OK  \n");
	TFHandle = (void *)1;
	init_pConfig(&pConfig);
	pConfig->FrameInfo->Height  = 0;
	ret32 = initDetectFaceIF(&TFHandle, pConfig);
	if(ret32 < 0 )
	{
		printf("ST_1_initDetectFaceIF_45 OK \n\n");
	}else{
		printf("ST_1_initDetectFaceIF_45 ERR \n\n");
	}


	printf("ST_1_initDetectFaceIF_46 \n");
	TFHandle = (void *)1;
	init_pConfig(&pConfig);
	pConfig->FrameInfo->Height  = 1920;
	ret32 = initDetectFaceIF(&TFHandle, pConfig);
	if(ret32 == 0 )
	{
		printf("ST_1_initDetectFaceIF_46 OK \n\n");
	}else{
		printf("ST_1_initDetectFaceIF_46 ERR \n\n");
	}



	printf("ST_1_initDetectFaceIF_47 \n");
	TFHandle = (void *)1;
	init_pConfig(&pConfig);
	pConfig->FrameInfo->ImgType  = 10;
	ret32 = initDetectFaceIF(&TFHandle, pConfig);
	if(ret32 < 0 )
	{
		printf("ST_1_initDetectFaceIF_47 OK \n\n");
	}else{
		printf("ST_1_initDetectFaceIF_47 ERR \n\n");
	}

	printf("ST_1_initDetectFaceIF_48 \n");
	TFHandle = (void *)1;
	init_pConfig(&pConfig);
	pConfig->FrameInfo->ImgType  = 3;
	ret32 = initDetectFaceIF(&TFHandle, pConfig);
	if( 0 == ret32  )
	{
		printf("ST_1_initDetectFaceIF_48 OK \n \n");
	}else{
		printf("ST_1_initDetectFaceIF_48 ERR \n\n");
	}


	printf("ST_1_initDetectFaceIF_49 \n");
	TFHandle = (void *)1;
	init_pConfig(&pConfig);
	UINT8 myImgText[256] = {0x00};
	memcpy(&(pConfig->FrameInfo->ImgText),&myImgText,256) ;
	ret32 = initDetectFaceIF(&TFHandle, pConfig);
	if( 0 == ret32  )
	{
		printf("ST_1_initDetectFaceIF_49 OK \n\n");
	}else{
		printf("ST_1_initDetectFaceIF_49 ERR \n\n");
	}
	

	printf("ST_1_initDetectFaceIF_50 \n");
	TFHandle = (void *)1;
	init_pConfig(&pConfig);
	pConfig->FrameInfo->TimeStamp = 0x3234;
	ret32 = initDetectFaceIF(&TFHandle, pConfig);
	if( 0 == ret32  )
	{
		printf("ST_1_initDetectFaceIF_50 OK \n\n");
	}else{
		printf("ST_1_initDetectFaceIF_50 ERR \n\n");
	}


	printf("ST_1_initDetectFaceIF_51 \n");
	TFHandle = (void *)1;
	init_pConfig(&pConfig);
	pConfig->FrameInfo->ImgLen = 1080*1920;
	ret32 = initDetectFaceIF(&TFHandle, pConfig);
	if( ret32 == 0  )
	{
		printf("ST_1_initDetectFaceIF_51 OK \n \n");
	}else{
		printf("ST_1_initDetectFaceIF_51 ERR \n\n");
	}


	printf("ST_1_initDetectFaceIF_52 \n");
	TFHandle = (void *)1;
	init_pConfig(&pConfig);
	pConfig->BuffSize = 0;
	ret32 = initDetectFaceIF(&TFHandle, pConfig);
	if( ret32  < 0  )
	{
		printf("ST_1_initDetectFaceIF_52 OK \n\n");
	}else{
		printf("ST_1_initDetectFaceIF_52 ERR \n\n");
	}

	printf("ST_1_initDetectFaceIF_53 \n");
	TFHandle = (void *)1;
	init_pConfig(&pConfig);
	pConfig->SerialPortInfoPtr->Baudrate = 123;
	ret32 = initDetectFaceIF(&TFHandle, pConfig);
	if( ret32 < 0 )
	{
		printf("ST_1_initDetectFaceIF_53 OK \n \n");
	}else{
		printf("ST_1_initDetectFaceIF_53 ERR \n\n");
	}

	printf("ST_1_initDetectFaceIF_54 \n");
	TFHandle = (void *)1;
	init_pConfig(&pConfig);
	pConfig->SrcUserInfo->UserName[0]='\0';
	ret32 = initDetectFaceIF(&TFHandle, pConfig);
	if( ret32 < 0 )
	{
		printf("ST_1_initDetectFaceIF_54 OK \n\n");
	}else{
		printf("ST_1_initDetectFaceIF_54 ERR \n\n");
	}

	printf("ST_1_initDetectFaceIF_55 \n");
	TFHandle = (void *)1;
	init_pConfig(&pConfig);
	pConfig->SrcUserInfo->UserPWD[0]='\0';
	ret32 = initDetectFaceIF(&TFHandle, pConfig);
	if( ret32 < 0 )
	{
		printf("ST_1_initDetectFaceIF_55 OK \n\n");
	}else{
		printf("ST_1_initDetectFaceIF_55 ERR \n\n");
	}

	printf("ST_1_initDetectFaceIF_56 \n");
	TFHandle = (void *)1;
	init_pConfig(&pConfig);
	pConfig->SrcIFInfo->CtrlPort = 0;
	ret32 = initDetectFaceIF(&TFHandle, pConfig);
	if( ret32 < 0 )
	{
		printf("ST_1_initDetectFaceIF_56 OK \n\n");
	}else{
		printf("ST_1_initDetectFaceIF_56 ERR \n\n");
	}

	printf("ST_1_initDetectFaceIF_57 \n");
	TFHandle = (void *)1;
	init_pConfig(&pConfig);
	for(int i = 0; i < 16; i++)
	{
		pConfig->SrcIFInfo->IPArray[i] = 0;
	}
	ret32 = initDetectFaceIF(&TFHandle, pConfig);
	if( ret32 < 0 )
	{
		printf("ST_1_initDetectFaceIF_57 OK \n\n");
	}else{
		printf("ST_1_initDetectFaceIF_57 ERR \n\n");
	}

	printf("ST_1_initDetectFaceIF_58 \n");
	TFHandle = (void *)1;
	init_pConfig(&pConfig);
	pConfig->SerialPortInfoPtr->Baudrate = 1000000;
	ret32 = initDetectFaceIF(&TFHandle, pConfig);
	if( 0 == ret32 )
	{
		printf("ST_1_initDetectFaceIF_58 OK \n \n");
	}else{
		printf("ST_1_initDetectFaceIF_58 ERR \n\n");
	}
	
	return 0;
}


int ST_1_initDetectFaceIF::run()
{
	return 0;
}


int ST_1_initDetectFaceIF::uninit()
{
	Driver::uninit();
}

ST_1_initDetectFaceIF::~ST_1_initDetectFaceIF()
{
	
}




	 
	

