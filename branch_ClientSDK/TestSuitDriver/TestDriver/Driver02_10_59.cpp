/********************************************************************
* Name      :  Driver02_10_59.cpp                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  Zhang Botong                                          
* Date      :  2018-07-02                                                 
********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "DetectFaceIF.h"
#include "Driver02_10_59.h"
#include <unistd.h>




Driver02_10_59::Driver02_10_59(unsigned int nCaseId ): Driver( nCaseId )
{

}

int Driver02_10_59::init()
{
	Driver::init();
}

int Driver02_10_59::run()
{
	lxTest();
	
	return 0;
}

int Driver02_10_59::uninit()
{
	Driver::uninit();
}

Driver02_10_59::~Driver02_10_59()
{
	
}

void Driver02_10_59::initRegion()

{
	printf("------- initRegion-----\n");

	void *TFHandle;
	int a = 566;
	TFHandle = &a;

	int nCount = 3;

	int data = 100;
	nCount = 4;	
	Point tempPoint1[4];
	tempPoint1[0].X = 0;	
	tempPoint1[0].Y = 0;	
	tempPoint1[1].X = data;	
	tempPoint1[1].Y = 0;
	tempPoint1[2].X = data;	
	tempPoint1[2].Y = data;
	tempPoint1[3].X = 0;	
	tempPoint1[3].Y = data;	
		
	printf("addDetectRegion nCount = %d\n",nCount);
	addDetectRegion( TFHandle, (Point *)(&tempPoint1),  nCount );
}




int Driver02_10_59:: initDetectCfgInfo(int DetectThreshold)
{
	printf("------- initDetectCfgInfo-----\n");
	int a = 55;
	void *TFHandle = &a;

	DetectCfg test;
	DetectCfg *pCfg = &test;

	pCfg = &test;
	pCfg->ConfigMask = MASK_DETECT_THRESHOLD;
	pCfg->DetectThreshold = 0;
	printf("[LX]initDetectCfgInfo : set pCfg->DetectThreshold = 0\n");
	setDetectConfig(TFHandle, pCfg);

	pCfg = &test;
	pCfg->ConfigMask = MASK_DETECT_TYPE;
	pCfg->DetectType= CAMERA_NOW_SNAP;
	pCfg->DetectThreshold = 1;
	printf("[LX]initDetectCfgInfo : set pCfg->DetectType = CAMERA_LEAVE_SNAP\n");
	setDetectConfig(TFHandle, pCfg);

	
}

void Driver02_10_59::lxTest()
{
	initRegion();
	initDetectCfgInfo(0);

	int ret32 = 0;
	RWFaceInfo Info;
	void *TFHandle;
	RWFaceInfo * tempRWFaceInfo =  &Info;
	tempRWFaceInfo->ImgID =  11;
	tempRWFaceInfo->TrackNo = 22;
	tempRWFaceInfo->RectX = 0;
	tempRWFaceInfo->RectY = 0;
	tempRWFaceInfo->RectWidth = 10;
	tempRWFaceInfo->RectHeight = 10;
	tempRWFaceInfo->RectType = 2;
	tempRWFaceInfo->QScore = 13;
		

	printf("\n\n ---------------- RealTimeCaptureStratege test ------------\n");
	tempRWFaceInfo->RectType = FACE_FRAME;
	tempRWFaceInfo->QScore = 35;
	tempRWFaceInfo->RectWidth = 20;
	tempRWFaceInfo->TrackNo = 25;
/*	ret32 = testFilterFace(tempRWFaceInfo);
	if(ret32 == 0)
	{
		testRefreshAfterFilter(tempRWFaceInfo);
	}
	//testsSelectFace(tempRWFaceInfo);
*/

}
	 
	

