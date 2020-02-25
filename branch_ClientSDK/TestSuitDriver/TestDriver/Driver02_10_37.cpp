/********************************************************************
* Name      :  Driver02_10_37.cpp                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  Zhang Botong                                          
* Date      :  2018-07-02                                                 
********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "DetectFaceIF.h"
#include "Driver02_10_37.h"


#define	 MASK_DETECT_TYPE 		(0x01)
#define  MASK_DETECT_HEART_BEAT (0x02) 
#define  MASK_DETECT_THRESHOLD 	(0x04)
#define	 MASK_BACK_GROUND_CTRL 	(0x08)
#define	 MASK_BACK_FACE_CTRL 	(0x10)
#define	 MASK_DETECT_TIME		(0x20)
#define	 MASK_FACE_SIZE			(0x40)


Driver02_10_37::Driver02_10_37(unsigned int nCaseId ): Driver( nCaseId )
{

}

int Driver02_10_37::init()
{
	Driver::init();
}

int Driver02_10_37::run()
{
	printf("Driver02_10_37 [test] run \n");
	LXTestWBS_02_10_37();
	
	return 0;
}

int Driver02_10_37::uninit()
{
	Driver::uninit();
}

Driver02_10_37::~Driver02_10_37()
{
	
}


void Driver02_10_37::LXTestWBS_02_10_37()
{
	printf("---test1 ---- LX:TestWBS_02_10_37 and TestWBS_02_10_39 and TestWBS_02_10_41 -----\n");
	void *TFHandle = getHandle();

	DetectCfg test;
	DetectCfg *pCfg = &test;

	pCfg->ConfigMask = MASK_FACE_SIZE;
	pCfg->FaceSize = 0x11;
	setDetectConfig(TFHandle, pCfg);
	UINT16 ret = getDetectConfig(TFHandle,pCfg);
	

	pCfg->ConfigMask = MASK_DETECT_TIME;
	pCfg->DetectTime = 0x22;
	setDetectConfig(TFHandle, pCfg);
    getDetectConfig(TFHandle,pCfg);

	pCfg->ConfigMask = MASK_FACE_SIZE;
	pCfg->FaceSize =  512;
	setDetectConfig(TFHandle, pCfg);
	getDetectConfig(TFHandle,pCfg);

	pCfg->ConfigMask = MASK_FACE_SIZE;
	pCfg->FaceSize =  513;
	setDetectConfig(TFHandle, pCfg);
	getDetectConfig(TFHandle,pCfg);

	
	
}
 
