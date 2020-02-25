/********************************************************************
* Name      :  Driver02_10_16.cpp                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  Zhang Botong                                          
* Date      :  2018-07-02                                                 
********************************************************************/
#include <stdio.h>
#include "DetectFaceIF.h"
#include "Driver02_10_16.h"

Driver02_10_16::Driver02_10_16(unsigned int nCaseId ): Driver( nCaseId )
{

}

int Driver02_10_16::init()
{
	Driver::init();
}

int Driver02_10_16::run()
{
	LXTestWBS_02_10_16();
	
	return 0;
}

int Driver02_10_16::uninit()
{
	Driver::uninit();
}

Driver02_10_16::~Driver02_10_16()
{
	
}

void Driver02_10_16:: LXTestWBS_02_10_16()
{

	printf("start---LXTestWBS_02_10_16 ----\n");
	int a = 55;
	void *TFHandle = &a;

	DetectCfg test;
	DetectCfg *pCfg = &test;

	pCfg = &test;
	pCfg->ConfigMask = 0x01;
	pCfg->DetectType = 0;
	a = 566;
	TFHandle = &a;
	printf("[LX]---set pCfg->DetectType = 0\n");
	setDetectConfig(TFHandle, pCfg);

	//pCfg = &test;
	//pCfg->ConfigMask = 0x01;
	pCfg->DetectType = 1;
	//a = 566;
	//TFHandle = &a;
	printf("[LX]---set pCfg->DetectType = 1\n");
	setDetectConfig(TFHandle, pCfg);

	pCfg->DetectType = 0;
	printf("[LX]---set pCfg->DetectType = 0\n");
	setDetectConfig(TFHandle, pCfg);

	printf("end---LXTestWBS_02_10_16 ----\n");
}
