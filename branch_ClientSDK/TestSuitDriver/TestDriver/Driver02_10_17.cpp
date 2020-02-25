/********************************************************************
* Name      :  Driver02_10_17.cpp                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  Zhang Botong                                          
* Date      :  2018-07-02                                                 
********************************************************************/

#include <stdio.h>
#include "DetectFaceIF.h"
#include "Driver02_10_17.h"

Driver02_10_17::Driver02_10_17(unsigned int nCaseId ): Driver( nCaseId )
{

}

int Driver02_10_17::init()
{
	Driver::init();
}

int Driver02_10_17::run()
{
	LXTestWBS_02_10_17();
	
	return 0;
}

int Driver02_10_17::uninit()
{
	Driver::uninit();
}

Driver02_10_17::~Driver02_10_17()
{
	
}

void Driver02_10_17::LXTestWBS_02_10_17()
{
	printf("---start---------------------------------------------test----WBS_02_10_17-------------------------\n");
	int a = 1;
	UINT64 pReqID = 56;
		
	/*
	  UINT8 *FileNamPtr;//文件名称字符串地址
	  UINT8 NameLen;//字符串长度（目前最大64）
	  UINT8 Algin[3];//字节对齐。不使用
	  UINT8 MD5[16];//心跳间隔时长，0：关闭对检测模块的心跳检测，正值：心跳检测间隔（默认为10秒）
	  UINT32 FileSize;
	*/
	UpgradeDataInfo DataInfo ;
		
	UpgradeDataInfo *pDataInfo = &DataInfo;
	UINT8 aaa[8]={0x55,0x55,0x55,0x55,0x55};
	pDataInfo->FileNamPtr = (UINT8 *) &aaa;
	pDataInfo->NameLen = 5;
	int i;
	for( i = 0 ; i < 16; i++)
	{
		pDataInfo->MD5[i] = i;
		printf(" %u ",pDataInfo->MD5[i]);
	}
	printf("\n");
	pDataInfo->FileSize = 1;
	CallbackReplyFunc *pFunc = (CallbackReplyFunc *)&testTest;
	printf("LXTestWBS_02_10_17 pDataInfo = %x,pDataInfo->FileSize = %u,  pDataInfo->F; \n",pDataInfo,pDataInfo->FileSize);
	//startUpgrade((void *)&a, &pReqID, pDataInfo,pFunc);
	printf("---end---------------------------------------------test----WBS_02_10_17-------------------------\n");
	
}

void Driver02_10_17::testTest(UINT64 *pReqID, UINT8 *pRet,
							  UINT8 *pReplyData,UINT32 *pDataLen)
{
  	printf("---the notify testfun" );
}

