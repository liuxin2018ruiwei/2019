/********************************************************************
* Name      :  Driver02_10_180.cpp                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  Zhang Botong                                          
* Date      :  2018-07-02                                                 
********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "DetectFaceIF.h"
#include "Driver02_10_180.h"
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




Driver02_10_180::Driver02_10_180(unsigned int nCaseId ): Driver( nCaseId )
{

}

int Driver02_10_180::init()
{
	Driver::init();
}

void testTest222(UINT64 *pReqID, UINT8 *pRet, UINT8 *pReplyData,UINT32 *pDataLen )
{
	printf("for fate!\n\n");
}

int Driver02_10_180::run()
{

	int ret32 = 0;

	printf("---start---------------------------------------------test----hanhui-------------------------\n");
	int a = 1;
	UINT64 pReqID = 56;
		
	UpgradeDataInfo DataInfo ;
		
	UpgradeDataInfo *pDataInfo = &DataInfo;

	const char* pFileName =  "Camera-QSPI-image.bin";
	pDataInfo->FileNamPtr = (UINT8*)pFileName;
	pDataInfo->NameLen = sizeof("Camera-QSPI-image.bin");
	const char* pFilePath = "/dev"; 
	pDataInfo->FilePathPtr = (UINT8*)pFilePath;
	pDataInfo->PathLen = sizeof("/dev");
	UINT8 tempMD5[16] = {	0x28,0xeb,0x7b,0x74,
							0x3e,0x87,0x93,0xd6,
							0x20,0xf2,0xfd,0x3a,
							0x97,0xa4,0x85,0xdc
					};

	

	printf("FilePathPtr = %s;name FileNamPtr = %s\n",pDataInfo->FileNamPtr,pDataInfo->FilePathPtr);
	int i;
	memcpy(pDataInfo->MD5, tempMD5,16);

	
	pDataInfo->FileSize = 6645704;
	
	CallbackReplyFunc pFunc = testTest222; 
	pDataInfo->UpgradeType = 2;
	printf("[LX] pDataInfo = %x,pDataInfo->FileSize = %u \n",pDataInfo,pDataInfo->FileSize);
	startUpgrade((void *)&a, &pReqID, pDataInfo,pFunc);
	
	printf("---end---------------------------------------------test----hanhui ok -------------------------\n");

/*	char fileTempName[128] = {0};
	memcpy(&fileTempName,"/dev/Camera-QSPI-image.bin", sizeof("/dev/Camera-QSPI-image.bin"));
	printf("fileTempName = %s\n\n",fileTempName);
	//-------------------test------
	testZmode(fileTempName);
	printf("---end---------------------------------------------test----hanhui ok -------------------------\n");
	while(1){};
	*/
}


int Driver02_10_180::uninit()
{
	Driver::uninit();
}

Driver02_10_180::~Driver02_10_180()
{
	
}




	 
	

