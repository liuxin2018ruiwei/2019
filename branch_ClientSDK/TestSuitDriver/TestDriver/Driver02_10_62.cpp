/********************************************************************
* Name      :  Driver02_10_62.cpp                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  Zhang Botong                                          
* Date      :  2018-07-02                                                 
********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "DetectFaceIF.h"
#include "Driver02_10_62.h"
#include <unistd.h>




Driver02_10_62::Driver02_10_62(unsigned int nCaseId ): Driver( nCaseId )
{

}

int Driver02_10_62::init()
{
	Driver::init();
}

int Driver02_10_62::run()
{
	lxTest();
	
	return 0;
}

int Driver02_10_62::uninit()
{
	Driver::uninit();
}

Driver02_10_62::~Driver02_10_62()
{
	
}

void Driver02_10_62::initRegion()

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




int Driver02_10_62:: initDetectCfgInfo(int DetectThreshold)
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

void Driver02_10_62::lxTest()
{
	int a = 55;
	void *TFHandle = &a;

	DetectCfg test;
	DetectCfg *pCfg = &test;


	printf("\n------------test--WBS_02_10_58-----\n");
	//gte MASK_DETECT_TYPE get DetectTime
	pCfg->DetectTime = 55;
	int ret32 = setDetectConfig(TFHandle, pCfg);
	pCfg->DetectTime = 00;
	pCfg->DetectType = 0x2;
	pCfg->ConfigMask = MASK_DETECT_TYPE;
	ret32 = getDetectConfig(TFHandle, pCfg);
	printf("pCfg->DetectType = %d;pCfg->DetectTime = %d\n",pCfg->DetectType,pCfg->DetectTime);
	printf("\n------------test--WBS_02_10_58 ok-----\n");


	printf("\n\n\n lxTest----------------WBS_02_10_62 ~ WBS_02_10_68 ------------\n");

	pCfg = &test;
	pCfg->ConfigMask = MASK_DETECT_TIME;
	pCfg->DetectThreshold = 0;
	pCfg->DetectTime = 12;
	printf("\n[LX]initDetectCfgInfo : set pCfg->DetectTime = %d\n",pCfg->DetectTime);
	ret32 = setDetectConfig(TFHandle, pCfg);
	if(ret32 == 0)
	{
		printf("set pCfg->DetectTime is ok !\n");
	}

	pCfg->DetectTime = 0;
	getDetectConfig(TFHandle, pCfg);
	printf("get pCfg->DetectTime = %d\n",pCfg->DetectTime);

	pCfg->DetectTime = 60;
	printf("\n[LX]initDetectCfgInfo : set pCfg->DetectTime = %d\n",pCfg->DetectTime);
	ret32 = setDetectConfig(TFHandle, pCfg);
	if(ret32 == 0)
	{
		printf("set pCfg->DetectTime is ok !\n");
	}
	pCfg->DetectTime = 0;
	getDetectConfig(TFHandle, pCfg);
	printf("get pCfg->DetectTime = %d\n",pCfg->DetectTime);



	pCfg->DetectTime = 61;
	printf("\n[LX]initDetectCfgInfo : set pCfg->DetectTime = %d\n",pCfg->DetectTime);
	ret32 = setDetectConfig(TFHandle, pCfg);
	if(ret32 == 0)
	{
		printf("set pCfg->DetectTime is ok !\n");
	}
	else
	{
		printf("set Err!\n");		
	}
	pCfg->DetectTime = 0;
	ret32 = getDetectConfig(TFHandle, pCfg);
	if(ret32 = 0)
	{
		printf("get pCfg->DetectTime = %d\n",pCfg->DetectTime);
	}else{
		printf("get Err \n");
	}


	pCfg->DetectTime = 0;
	printf("\n[LX]initDetectCfgInfo : set pCfg->DetectTime = %d\n",pCfg->DetectTime);
	ret32 = setDetectConfig(TFHandle, pCfg);
	if(ret32 == 0)
	{
		printf("set pCfg->DetectTime is ok !\n");
	}
	else
	{
		printf("set Err!\n");		
	}
	pCfg->DetectTime = 0;
	ret32 = getDetectConfig(TFHandle, pCfg);
	if(ret32 == 0)
	{
		printf("get pCfg->DetectTime = %d\n",pCfg->DetectTime);
	}
	else
	{
		printf("get Err \n");
	}
	printf("\n\n\n lxTest----------------WBS_02_10_62 ~ WBS_02_10_68 -----ok-------\n");



	printf("\n------------test--WBS_02_10_69~WBS_02_10_71-----\n");
	pCfg = &test;
	pCfg->ConfigMask = MASK_DETECT_THRESHOLD;
	pCfg->DetectThreshold = 0;
	printf("\n[LX]initDetectCfgInfo : set pCfg->DetectThreshold = %d\n",pCfg->DetectThreshold);
	ret32 = setDetectConfig(TFHandle, pCfg);
	if(ret32 == 0)
	{
		printf("set pCfg->DetectThreshold is ok !\n");
	}
	pCfg->DetectThreshold = 5;
	getDetectConfig(TFHandle, pCfg);
	printf("get pCfg->DetectThreshold = %d\n",pCfg->DetectThreshold);

	pCfg->DetectThreshold = 100;
	printf("\n[LX]initDetectCfgInfo : set pCfg->DetectThreshold = %d\n",pCfg->DetectThreshold);
	ret32 = setDetectConfig(TFHandle, pCfg);
	if(ret32 == 0)
	{
		printf("set pCfg->DetectThreshold is ok !\n");
	}
	pCfg->DetectThreshold = 0;
	getDetectConfig(TFHandle, pCfg);
	printf("get pCfg->DetectThreshold = %d\n",pCfg->DetectThreshold);


	pCfg->DetectThreshold = 101;
	printf("\n[LX]initDetectCfgInfo : set pCfg->DetectThreshold = %d\n",pCfg->DetectThreshold);
	ret32 = setDetectConfig(TFHandle, pCfg);
	if(ret32 == 0)
	{
		printf("set pCfg->DetectThreshold is ok !\n");
	}
	pCfg->DetectThreshold = 0;
	getDetectConfig(TFHandle, pCfg);
	printf("get pCfg->DetectThreshold = %d\n",pCfg->DetectThreshold);
	printf("\n------------test--WBS_02_10_69~WBS_02_10_71----ok-\n");


	printf("\n-----------------------------------------------test--WBS_02_10_69~WBS_02_10_72-----\n");
	
	pCfg->DetectThreshold = 27;
	printf("\n[LX]initDetectCfgInfo : set pCfg->DetectThreshold = %d\n",pCfg->DetectThreshold);
	ret32 = setDetectConfig(TFHandle, pCfg);
	if(ret32 == 0)
	{
		printf("set pCfg->DetectThreshold is ok !\n");
	}

	
	initRegion();
	RWFaceInfo Info;
	RWFaceInfo * tempRWFaceInfo =  &Info;
	tempRWFaceInfo->ImgID =  11;
	tempRWFaceInfo->TrackNo = 22;
	tempRWFaceInfo->RectX = 2;
	tempRWFaceInfo->RectY = 2;
	tempRWFaceInfo->RectWidth = 10;
	tempRWFaceInfo->RectHeight = 10;
	tempRWFaceInfo->RectType = 2;
	
	tempRWFaceInfo->QScore = 13;
	tempRWFaceInfo->TrackNo = 22;
	printf("[LX]tempRWFaceInfo->TrackNo = %d,tempRWFaceInfo->RectWidth  = %d\n",tempRWFaceInfo->TrackNo,tempRWFaceInfo->RectWidth );

	tempRWFaceInfo->RectType = FACE_FRAME;
/*	ret32 = testFilterFace(tempRWFaceInfo);
	if(ret32 == 0)
	{
		printf("testFilterFace is ok !\n");
	}


	tempRWFaceInfo->QScore = 66;
	tempRWFaceInfo->RectWidth = 27;

	printf("\n[LX]initDetectCfgInfo : set pCfg->DetectThreshold = %d\n",pCfg->DetectThreshold);
	ret32 = setDetectConfig(TFHandle, pCfg);
	
	ret32 = testFilterFace(tempRWFaceInfo);
	if(ret32 == 0)
	{
		printf("testFilterFace is ok !\n");
	}


	pCfg->DetectThreshold = 27;
	tempRWFaceInfo->QScore = 23;
	printf("\n[LX]initDetectCfgInfo : set pCfg->DetectThreshold = %d,tempRWFaceInfo->QScore = %f\n",pCfg->DetectThreshold,tempRWFaceInfo->QScore);
	ret32 = setDetectConfig(TFHandle, pCfg);
	ret32 = testFilterFace(tempRWFaceInfo);
	if(ret32 == 0)
	{
		printf("testFilterFace is ok !\n");
	}

	
	pCfg->DetectThreshold = 27;
	tempRWFaceInfo->QScore = 66;
	printf("\n[LX]initDetectCfgInfo : set pCfg->DetectThreshold = %d,tempRWFaceInfo->QScore = %f\n",pCfg->DetectThreshold,tempRWFaceInfo->QScore);
	ret32 = setDetectConfig(TFHandle, pCfg);
	ret32 = testFilterFace(tempRWFaceInfo);
	if(ret32 == 0)
	{
		printf("testFilterFace is ok !\n");
	}
	printf("\n------------test--WBS_02_10_69~WBS_02_10_75-----\n");


	printf("\n------------test--WBS_02_10_76~WBS_02_10_77-----\n");
	
	pCfg = &test;
	pCfg->ConfigMask = MASK_BACK_GROUND_CTRL;
	pCfg->BackgroundCtrl= 11;
	printf("\nset pCfg->BackgroundCtrl = %d\n",pCfg->BackgroundCtrl);
	ret32 = setDetectConfig(TFHandle, pCfg);
	if(ret32 == 0)
	{
		printf("set pCfg->BackgroundCtrl is ok !\n");
	}

	pCfg->BackgroundCtrl = 0;
	getDetectConfig(TFHandle, pCfg);
	printf("get pCfg->BackgroundCtrl = %d\n",pCfg->BackgroundCtrl);
	
	printf("\n------------test--WBS_02_10_76~WBS_02_10_77---ok--\n");


	printf("\n------------test--WBS_02_10_58~WBS_02_10_59-----\n");
	pCfg->ConfigMask = MASK_DETECT_TYPE;
	tempRWFaceInfo->RectType = FACE_FRAME;
	pCfg->DetectType = CAMERA_INTERVAL_SNAP;
	tempRWFaceInfo->QScore = 88;
	ret32 = setDetectConfig(TFHandle, pCfg);
	//ret32 = testFilterFace(tempRWFaceInfo);
	testRefreshAfterFilter(tempRWFaceInfo);
	testsSelectFace(tempRWFaceInfo);

	tempRWFaceInfo->QScore = 99;
	testRefreshAfterFilter(tempRWFaceInfo);
	testsSelectFace(tempRWFaceInfo);
	*/
	printf("\n------------test--WBS_02_10_58~WBS_02_10_59--ok---\n");
}
	 
	

