/********************************************************************
* Name      :  Driver02_10_01.cpp                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  Zhang Botong                                          
* Date      :  2018-07-02                                                 
********************************************************************/

#include <stdio.h>
#include "DetectFaceIF.h"
#include "Driver02_10_01.h"

Driver02_10_01::Driver02_10_01(unsigned int nCaseId ): Driver( nCaseId )
{

}

int Driver02_10_01::init()
{
	Driver::init();
}

int Driver02_10_01::run()
{
	LXTestWBS_02_10_01();
	return 0;
}

int Driver02_10_01::uninit()
{
	Driver::uninit();
}

Driver02_10_01::~Driver02_10_01()
{
	
}

void Driver02_10_01::LXTestWBS_02_10_01()
{
		
	//printf("---start---------------------------------------------test----WBS_02_10_01-------------------------");
	//test1 --空值判断--
	printf("---test1 ---- set setDetectConfig :TFHandle = nullptr ;pCfg = nullptr -----\n");
	int a = 55;
	void *TFHandle = getHandle();

	DetectCfg test;
	DetectCfg *pCfg = &test;
			
	TFHandle = nullptr;
	pCfg = nullptr;
	setDetectConfig(TFHandle, pCfg);
	printf("---\n");
		
	//test2----WBS_02_10_01----
	printf("---test2 ---- set setDetectConfig :TFHandle = nullptr ;pCfg = 55-----\n");
	TFHandle = 0;
	pCfg = &test;
	pCfg->ConfigMask = 0x02;
	pCfg->DetectHeartbeat = 55;
	setDetectConfig(TFHandle, pCfg);
	getDetectConfig( TFHandle, pCfg );
	printf("---test2 get pcf->DetectHeartbeat = %d\n\n",pCfg->DetectHeartbeat);
		
	//test3----WBS_02_10_01----
	printf("---test3 ---- set setDetectConfig:TFHandle = 55 ;pCfg = nullptr -----\n");
		
	TFHandle = &a;
	pCfg = 0;
	setDetectConfig(TFHandle, pCfg);
	printf("---\n");
		
	//test4 --正常输入0判断--
	printf("---test4 ---- set setDetectConfig ::TFHandle = 565 ;pCfg->DetectHeartbeat = 0-----\n");
	pCfg = &test;
	pCfg->ConfigMask = 0x02;
	pCfg->DetectHeartbeat = 0;
	a = 565;
	TFHandle = &a;
	setDetectConfig(TFHandle, pCfg);
	getDetectConfig( TFHandle, pCfg );
	printf("---test4 get pcf->DetectHeartbeat = %d\n\n",pCfg->DetectHeartbeat);
	//test3 --正常输入判断--
	printf("---test5 ---- set setDetectConfig :TFHandle = 565 ;pCfg->DetectHeartbeat = 66-----\n");
	pCfg = &test;
	pCfg->ConfigMask = 0x02;
	pCfg->DetectHeartbeat = 66;
	a = 565;
	setDetectConfig(TFHandle, pCfg);
	getDetectConfig( TFHandle, pCfg );
	printf("---test5 get pcf->DetectHeartbeat = %d\n",pCfg->DetectHeartbeat);
	printf("---end---------------------------------------------test----WBS_02_10_01-------------------------\n\n");



	printf("---start---------------------------------------------test----WBS_02_10_02-------------------------\n");
	printf("---test1 ---- set setDetectConfig :TFHandle = 566 ;pCfg->DetectHeartbeat = 77,pCfg->ConfigMask = 0x00-----\n");
	pCfg = &test;
	pCfg->ConfigMask = 0x00;
	pCfg->DetectHeartbeat = 77;
	a = 566;
	TFHandle = &a;
	printf("---set pCfg->DetectHeartbeat = 77\n");
	setDetectConfig(TFHandle, pCfg);
		
		
	getDetectConfig( TFHandle, pCfg );
	printf("---test1 get pcf->DetectHeartbeat = %d\n\n",pCfg->DetectHeartbeat);
		
	printf("---test2 ---- set setDetectConfig :TFHandle = 566 ;pCfg->DetectHeartbeat = 88,pCfg->ConfigMask = 0x02-----\n");
	pCfg = &test;
	pCfg->ConfigMask = 0x02;
	pCfg->DetectHeartbeat = 88;
	a = 566;
	TFHandle = &a;
	printf("---set pCfg->DetectHeartbeat = 88,pCfg->ConfigMask = %d\n",pCfg->ConfigMask);
	setDetectConfig(TFHandle, pCfg);
		
	getDetectConfig( TFHandle, pCfg );
	printf("---test2 get pcf->DetectHeartbeat = %d\n",pCfg->DetectHeartbeat);
	printf("---end---------------------------------------------test----WBS_02_10_02-------------------------\n");





	printf("---start---------------------------------------------test----WBS_02_10_03-------------------------\n");
	printf("---test1 ---- set setDetectConfig :TFHandle = 566 ;pCfg->DetectHeartbeat = 99,pCfg->ConfigMask = 0x02-----\n");
	pCfg = &test;
	pCfg->ConfigMask = 0x02;
	pCfg->DetectHeartbeat = 99;
	a = 566;
	printf("---set pCfg->DetectHeartbeat = 77\n");
	setDetectConfig(TFHandle, pCfg);
	getDetectConfig( TFHandle, pCfg );
	printf("---test1 get pcf->DetectHeartbeat = %d\n\n",pCfg->DetectHeartbeat);
		
	printf("---test2 ---- set setDetectConfig :TFHandle = 566 ;pCfg->DetectHeartbeat = 0,pCfg->ConfigMask = 0x02-----\n");
	pCfg = &test;
	pCfg->ConfigMask = 0x02;
	pCfg->DetectHeartbeat = 0;
	a = 566;
	printf("---set pCfg->DetectHeartbeat = 0\n");
	setDetectConfig(TFHandle, pCfg);
		
	getDetectConfig( TFHandle, pCfg );
	printf("---test1 get pcf->DetectHeartbeat = %d\n\n",pCfg->DetectHeartbeat);
		
	printf("---test2 ---- set setDetectConfig :TFHandle = 566 ;pCfg->DetectHeartbeat = 255,pCfg->ConfigMask = 0x02-----\n");
	pCfg = &test;
	pCfg->ConfigMask = 0x02;
	pCfg->DetectHeartbeat = 5;
	a = 566;
	printf("---set pCfg->DetectHeartbeat = 255\n");
	setDetectConfig(TFHandle, pCfg);
		

	getDetectConfig( TFHandle, pCfg );
	printf("---test1 get pcf->DetectHeartbeat = %d\n\n",pCfg->DetectHeartbeat);

	printf("---end---------------------------------------------test----WBS_02_10_03-------------------------\n");

	//-----------test----WBS_02_10_01----
	
}
