/********************************************************************
* Name      :  Driver02_10_05.cpp                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  Zhang Botong                                          
* Date      :  2018-07-02                                                 
********************************************************************/

#include <stdio.h>
#include "DetectFaceIF.h"
#include "Driver02_10_05.h"

Driver02_10_05::Driver02_10_05(unsigned int nCaseId ): Driver( nCaseId )
{

}

int Driver02_10_05::init()
{
	Driver::init();
}

int Driver02_10_05::run()
{

	LXTestWBS_02_10_05();
	
	return 0;
}

int Driver02_10_05::uninit()
{
	Driver::uninit();
}

Driver02_10_05::~Driver02_10_05()
{
	
}

void Driver02_10_05::LXTestWBS_02_10_05()
{
	int a = 55;
	void *TFHandle = &a;

	DetectCfg test;
	DetectCfg *pCfg = &test;

	pCfg = &test;
	pCfg->ConfigMask = 0x02;
	pCfg->DetectHeartbeat = 77;
	a = 566;
	TFHandle = &a;
	printf("---set pCfg->DetectHeartbeat = 77\n");
	setDetectConfig(TFHandle, pCfg);


	pCfg = &test;
	pCfg->ConfigMask = 0x01;

	pCfg->DetectHeartbeat = 77;
	pCfg->DetectType = 5;
	a = 566;
	TFHandle = &a;

	printf("---set pCfg->DetectType = 5\n");
	setDetectConfig(TFHandle, pCfg);

}
  
