/********************************************************************
* Name      :  Driver02_10_39.cpp                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  Zhang Botong                                          
* Date      :  2018-07-02                                                 
********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "DetectFaceIF.h"
#include "Driver02_10_39.h"
#include <unistd.h>




Driver02_10_39::Driver02_10_39(unsigned int nCaseId ): Driver( nCaseId )
{

}

int Driver02_10_39::init()
{
	Driver::init();
}

int Driver02_10_39::run()
{
	//printf("Driver02_10_39 [test] run \n");
	lxTest();
	
	return 0;
}

int Driver02_10_39::uninit()
{
	Driver::uninit();
}

Driver02_10_39::~Driver02_10_39()
{
	
}

void Driver02_10_39::initRegion()

{
	printf("------- initRegion-----\n");

	void *TFHandle;
	int a = 566;
	TFHandle = &a;

	int nCount = 3;


/*	
	nCount = 3;
	Point tempPoint[3];
	tempPoint[0].X = 30;	
	tempPoint[0].Y = 20;	
	tempPoint[1].X = 40;	
	tempPoint[1].Y = 20;
	tempPoint[2].X = 20;	
	tempPoint[2].Y = 40;
	printf("addDetectRegion nCount = %d\n",nCount);
	addDetectRegion( TFHandle, (Point *)(&tempPoint),  nCount );
	*/

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

int Driver02_10_39:: initDetectCfgInfo(int DetectThreshold)
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
	pCfg->DetectType= CAMERA_LEAVE_SNAP;
	printf("[LX]initDetectCfgInfo : set pCfg->DetectType = CAMERA_LEAVE_SNAP\n");
	setDetectConfig(TFHandle, pCfg);

	
}

void Driver02_10_39::lxTest()
{
	initRegion();
	initDetectCfgInfo(0);

	int ret32 = 0;
	RWFaceInfo Info;
	RWFaceInfo * tempRWFaceInfo =  &Info;
	tempRWFaceInfo->ImgID =  11;
	tempRWFaceInfo->TrackNo = 22;
	tempRWFaceInfo->RectX = 0;
	tempRWFaceInfo->RectY = 0;
	tempRWFaceInfo->RectWidth = 10;
	tempRWFaceInfo->RectHeight = 10;
	tempRWFaceInfo->RectType = 2;
	tempRWFaceInfo->QScore = 13;
		
	//FaceDetector::filterFace
/*	printf(" \n\n----------------1.testFilterFace  \n");
	testFilterFace(tempRWFaceInfo);
	printf(" ----------------1.testFilterFace  ok\n");

	tempRWFaceInfo->ImgID =  11;
	tempRWFaceInfo->TrackNo = 22;
	tempRWFaceInfo->RectX = 1000;
	tempRWFaceInfo->RectY = 1000;
	tempRWFaceInfo->RectWidth = 10;
	tempRWFaceInfo->RectHeight = 10;
	tempRWFaceInfo->RectType = 2;
	tempRWFaceInfo->QScore = 13;
	//FaceDetector::filterFace
	printf("\n\n ----------------1.testFilterFace test no in regin  \n");
	testFilterFace(tempRWFaceInfo);
	printf(" ----------------1.testFilterFace  ok\n\n");



	tempRWFaceInfo->ImgID =  11;
	tempRWFaceInfo->TrackNo = 22;
	tempRWFaceInfo->RectX = 2;
	tempRWFaceInfo->RectY = 2;
	tempRWFaceInfo->RectWidth = 10;
	tempRWFaceInfo->RectHeight = 10;
	tempRWFaceInfo->RectType = 2;
	tempRWFaceInfo->QScore = 13;
	//FaceDetector::filterFace

	printf("\n\n ----------------1.CaptureThresholdFilter test	\n");
	int a = 55;
	void *TFHandle = &a;
	DetectCfg test;
	DetectCfg *pCfg = &test;
	pCfg->ConfigMask = MASK_DETECT_THRESHOLD;
	pCfg->DetectThreshold = 0;
	printf("[LX]initDetectCfgInfo : set pCfg->DetectThreshold = 0\n");
	setDetectConfig(TFHandle, pCfg);
	testFilterFace(tempRWFaceInfo);
	printf(" ----------------1.testFilterFace  ok\n\n");


	printf("\n\n ----------------1.CaptureThresholdFilter test  	\n");
	//DetectCfg test;
	pCfg = &test;
	pCfg->ConfigMask = MASK_DETECT_THRESHOLD;
	pCfg->DetectThreshold = 5;
	tempRWFaceInfo->TrackNo = 22;
	printf("[LX]initDetectCfgInfo : set pCfg->DetectThreshold = 5;RectType = FACE_FRAME\n");
	setDetectConfig(TFHandle, pCfg);
	tempRWFaceInfo->RectType = FACE_FRAME;
	testFilterFace(tempRWFaceInfo);
	tempRWFaceInfo->QScore = 17;
	printf(" ----------------1.testFilterFace  ok\n\n");



	printf("\n\n ----------------1.CaptureThresholdFilter test	\n");
	pCfg->ConfigMask = MASK_DETECT_THRESHOLD;
	pCfg->DetectThreshold = 5;
	printf("[LX]initDetectCfgInfo : set pCfg->DetectThreshold = 5, but RectType HEAD_HOULDER_FRAME\n");
	setDetectConfig(TFHandle, pCfg);
	tempRWFaceInfo->RectType = HEAD_HOULDER_FRAME;
	tempRWFaceInfo->QScore = 27;
	testFilterFace(tempRWFaceInfo);

	printf(" ----------------1.testFilterFace  ok\n\n");

	
	
	printf("\n\n ----------------1. testFilterFace FaceSizeFilter  FaceSize < RectWidth	\n");
	pCfg->ConfigMask = MASK_FACE_SIZE;
	pCfg->FaceSize = 15;
	setDetectConfig(TFHandle, pCfg);
	tempRWFaceInfo->RectType = FACE_FRAME;
	tempRWFaceInfo->QScore = 27;
	tempRWFaceInfo->RectWidth = 10;
	testFilterFace(tempRWFaceInfo);
	printf(" ----------------1.testFilterFace  ok\n\n");


	printf("\n\n ----------------1. testFilterFace FaceSizeFilter  FaceSize > RectWidth	\n");
	pCfg->ConfigMask = MASK_FACE_SIZE;
	pCfg->FaceSize = 15;
	setDetectConfig(TFHandle, pCfg);
	tempRWFaceInfo->RectType = FACE_FRAME;
	tempRWFaceInfo->QScore = 27;
	tempRWFaceInfo->RectWidth = 20;
	testFilterFace(tempRWFaceInfo);
	printf(" ----------------1.testFilterFace  ok\n\n");

	printf("\n\n ----------------1. saveFaceNode test updata ok	\n");
	pCfg->ConfigMask = MASK_FACE_SIZE;
	pCfg->FaceSize = 15;
	setDetectConfig(TFHandle, pCfg);
	tempRWFaceInfo->RectType = FACE_FRAME;
	tempRWFaceInfo->QScore = 33;
	tempRWFaceInfo->RectWidth = 20;
	ret32 = testFilterFace(tempRWFaceInfo);
	if(ret32 == 0)
	{
		printf("ret32 = %d,tempRWFaceInfo->QScore =%f,tempRWFaceInfo = %x \n",ret32,tempRWFaceInfo->QScore,tempRWFaceInfo);
		testRefreshAfterFilter(tempRWFaceInfo);
	}
	printf(" ----------------1.saveFaceNode  ok\n\n");

	
	printf("\n\n ----------------1. updata test \n");
	tempRWFaceInfo->RectType = FACE_FRAME;
	tempRWFaceInfo->QScore = 34;
	tempRWFaceInfo->RectWidth = 20;
	ret32 = testFilterFace(tempRWFaceInfo);
	if(ret32 == 0)
	{
		printf("ret32 = %d,tempRWFaceInfo->QScore =%f \n",ret32,tempRWFaceInfo->QScore);
		testRefreshAfterFilter(tempRWFaceInfo);
	}
	printf("\n\n ----------------1. updata test  ok	\n");

	printf("\n\n ----------------1. updata2 test \n");
	tempRWFaceInfo->RectType = FACE_FRAME;
	tempRWFaceInfo->QScore = 35;
	tempRWFaceInfo->RectWidth = 20;
	ret32 = testFilterFace(tempRWFaceInfo);
	if(ret32 == 0)
	{
		printf("ret32 = %d,tempRWFaceInfo->QScore =%f,tempRWFaceInfo =%x \n",ret32,tempRWFaceInfo->QScore,tempRWFaceInfo);
		testRefreshAfterFilter(tempRWFaceInfo);
	}
	printf("\n\n ----------------1. updata2 test  ok	\n");


	printf("\n\n ---------------- add Node test ------------\n");
	tempRWFaceInfo->RectType = FACE_FRAME;
	tempRWFaceInfo->QScore = 35;
	tempRWFaceInfo->RectWidth = 20;
	tempRWFaceInfo->TrackNo = 25;
	ret32 = testFilterFace(tempRWFaceInfo);
	if(ret32 == 0)
	{
		printf("ret32 = %d,tempRWFaceInfo->QScore =%f,tempRWFaceInfo =%x \n",ret32,tempRWFaceInfo->QScore,tempRWFaceInfo);
		testRefreshAfterFilter(tempRWFaceInfo);
	}
	printf("\n\n ---------------- add Node test ok ------------\n");

	printf("\n\n ---------------- add Node test2 ------------\n");
	tempRWFaceInfo->RectType = FACE_FRAME;
	tempRWFaceInfo->QScore = 35;
	tempRWFaceInfo->RectWidth = 20;
	tempRWFaceInfo->TrackNo = 25;
	ret32 = testFilterFace(tempRWFaceInfo);
	if(ret32 == 0)
	{
		printf("ret32 = %d,tempRWFaceInfo->QScore =%f,tempRWFaceInfo =%x \n",ret32,tempRWFaceInfo->QScore,tempRWFaceInfo);
		testRefreshAfterFilter(tempRWFaceInfo);
	}
	printf("\n\n ---------------- add Node test2 ok ------------\n");

	printf("\n\n ---------------- add Node test2 ------------\n");
	tempRWFaceInfo->RectType = FACE_FRAME;
	tempRWFaceInfo->QScore = 35;
	tempRWFaceInfo->RectWidth = 20;
	tempRWFaceInfo->TrackNo = 3;
	ret32 = testFilterFace(tempRWFaceInfo);
	if(ret32 == 0)
	{
		printf("ret32 = %d,tempRWFaceInfo->QScore =%f,tempRWFaceInfo =%x \n",ret32,tempRWFaceInfo->QScore,tempRWFaceInfo);
		testRefreshAfterFilter(tempRWFaceInfo);
	}
	printf("\n\n ---------------- add Node test3 ok ------------\n");

	printf("\n\n ---------------- add Node test4 ------------\n");
	tempRWFaceInfo->RectType = FACE_FRAME;
	tempRWFaceInfo->QScore = 35;
	tempRWFaceInfo->RectWidth = 20;
	tempRWFaceInfo->TrackNo = 65;
	ret32 = testFilterFace(tempRWFaceInfo);
	if(ret32 == 0)
	{
		printf("ret32 = %d,tempRWFaceInfo->QScore =%f,tempRWFaceInfo =%x \n",ret32,tempRWFaceInfo->QScore,tempRWFaceInfo);
		testRefreshAfterFilter(tempRWFaceInfo);
	}
	printf("\n\n ---------------- add Node test3 ok ------------\n");
	

	printf("\n\n ----------------2. SelectFace  test delete: sleep 3s	\n");
	sleep(3);
	//testsSelectFace(tempRWFaceInfo);
	*/
	printf("\n\n ----------------2. SelectFace  ok	\n");		
}
	 
	

