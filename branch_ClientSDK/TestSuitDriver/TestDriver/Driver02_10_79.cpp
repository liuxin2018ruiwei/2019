/********************************************************************
* Name      :  Driver02_10_79.cpp                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  Zhang Botong                                          
* Date      :  2018-07-02                                                 
********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "DetectFaceIF.h"
#include "Driver02_10_79.h"
#include <unistd.h>




Driver02_10_79::Driver02_10_79(unsigned int nCaseId ): Driver( nCaseId )
{

}

int Driver02_10_79::init()
{
	Driver::init();
}

int Driver02_10_79::run()
{
	lxTest();
	
	return 0;
}

int Driver02_10_79::uninit()
{
	Driver::uninit();
}

Driver02_10_79::~Driver02_10_79()
{
	
}

void Driver02_10_79::initRegion()

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




int Driver02_10_79:: initDetectCfgInfo(int DetectThreshold)
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

void Driver02_10_79::lxTest()
{
	int ret32 = 0;
	int a = 55;
	void *TFHandle = &a;

	DetectCfg test;
	DetectCfg *pCfg = &test;


	
	pCfg = &test;
	pCfg->ConfigMask = MASK_BACK_GROUND_CTRL|MASK_DETECT_TYPE|MASK_FACE_CTRL;
	pCfg->BackgroundCtrl= 11;
	pCfg->DetectType = CAMERA_LEAVE_SNAP;
	pCfg->FaceCtrl = 0x12;
	printf("\n++set pCfg->ConfigMask = 0x%x; pCfg->BackgroundCtrl = 0x%x,pCfg->FaceCtrl = 0x%x\n",pCfg->ConfigMask,pCfg->BackgroundCtrl,pCfg->FaceCtrl);
	ret32 = setDetectConfig(TFHandle, pCfg);
	getDetectConfig(TFHandle, pCfg);
	printf("\n++get pCfg->BackgroundCtrl = 0x%x,pCfg->FaceCtrl = 0x%x\n",pCfg->BackgroundCtrl,pCfg->FaceCtrl);


	initRegion();
	RWFaceInfo Info;
	RWFaceInfo * tempRWFaceInfo =  &Info;
	tempRWFaceInfo->ImgID =  11;
	tempRWFaceInfo->TrackNo = 22;
	tempRWFaceInfo->RectX = 2;
	tempRWFaceInfo->RectY = 2;
	tempRWFaceInfo->RectWidth = 10;
	tempRWFaceInfo->RectHeight = 10;
	
	
	tempRWFaceInfo->QScore = 13;
	tempRWFaceInfo->TrackNo = 22;
	printf("[LX]tempRWFaceInfo->TrackNo = %d,tempRWFaceInfo->RectWidth  = %d\n",tempRWFaceInfo->TrackNo,tempRWFaceInfo->RectWidth );
	tempRWFaceInfo->RectType = HEAD_HOULDER_FRAME;
	
	//testRefreshAfterFilter(tempRWFaceInfo);
	//testsSelectFace(tempRWFaceInfo);



	tempRWFaceInfo->TrackNo = 33;
	tempRWFaceInfo->RectType = FACE_RECT;
	
	//testRefreshAfterFilter(tempRWFaceInfo);
	//testsSelectFace(tempRWFaceInfo);
	pCfg->ConfigMask = MASK_FACE_CTRL;
	tempRWFaceInfo->RectType = FACE_SHOULD_RECT;
	pCfg->FaceCtrl= COMPRESSION_60_FACE_MAP | FACE_MAP;
	printf("[LX]tempRWFaceInfo->TrackNo = %d, pCfg->FaceCtrl = 0x%x\n",tempRWFaceInfo->TrackNo,pCfg->FaceCtrl );
	printf("\nset pCfg->BackgroundCtrl = %d\n",pCfg->BackgroundCtrl);
	ret32 = setDetectConfig(TFHandle, pCfg);

	printf("\n------------test--WBS_02_10_79-----\n");
	
	
	if(ret32 == 0)
	{
		printf("set pCfg->BackgroundCtrl is ok !\n");
	}

	pCfg->BackgroundCtrl = 0;
	getDetectConfig(TFHandle, pCfg);
	printf("get pCfg->BackgroundCtrl = %d\n",pCfg->BackgroundCtrl);
	
	printf("\n------------test-all DetectConfig set and get \n");

	pCfg->ConfigMask = 0xFFFF;//MASK_FACE_CTRL|MASK_BACK_GROUND_CTRL;
	//tempRWFaceInfo->RectType = FACE_SHOULD_RECT;
	pCfg->FaceCtrl= 0x11;
	pCfg->BackgroundCtrl = 0x12;
	pCfg->DetectHeartbeat = 0x13;
	pCfg->DetectThreshold = 0x14;
	pCfg->DetectTime = 0x15;
	pCfg->DetectType = 2;
	pCfg->FaceSize = 0x17;
	//printf("[LX]tempRWFaceInfo->TrackNo = %d, pCfg->FaceCtrl = 0x%x,pCfg->BackgroundCtrl = %x\n",tempRWFaceInfo->TrackNo,pCfg->FaceCtrl,pCfg->BackgroundCtrl );
	//printf("\nset pCfg->BackgroundCtrl = %d\n",pCfg->BackgroundCtrl);
	ret32 = setDetectConfig(TFHandle, pCfg);
	pCfg->BackgroundCtrl = 0;
	pCfg->FaceCtrl=0;
	pCfg->DetectHeartbeat = 0x00;
	pCfg->DetectThreshold = 0x00;
	pCfg->DetectTime = 0x00;
	pCfg->DetectType = 0x00;
	pCfg->FaceSize = 0x00;
	getDetectConfig(TFHandle, pCfg);
	printf("\nget pCfg->BackgroundCtrl = %x\n",pCfg->BackgroundCtrl);
	printf("\nget pCfg->FaceCtrl = %x\n",pCfg->FaceCtrl);

	

	printf("\ntest  addFrame\n");
	/********/
	int hight = 10;
	int width = 19;
	int len = hight *width+8;
	
	UINT8 * pic = new UINT8[len];
	int ID = 23;
	writeInt32(pic,ID);
	if(0 == addFrame( TFHandle,pic))
		printf("pic1 = %x\n",pic);

	//pic = new UINT8[len];
	ID = 24;
	UINT8 * pic2 = new UINT8[len];
	writeInt32(pic2,ID);
	if(0 == addFrame( TFHandle,pic2))
	 	printf("pic2 = %x\n",pic2);

	ID = 25;
	UINT8 * pic3 = new UINT8[len];
	writeInt32(pic3,ID);
	if(0 == addFrame( TFHandle,pic3))
		printf("pic3 = %x\n",pic3);

	ID = 26;
	UINT8 * pic4 = new UINT8[len];
	writeInt32(pic4,ID);
	if(0 == addFrame( TFHandle,pic4))
	 	printf("pic4 = %x\n",pic4);
	
	ID = 23;
	UINT8 * pic5 = new UINT8[len];
	writeInt32(pic5,ID);
	if(0 == addFrame( TFHandle,pic5))
		printf("pic5 = %x\n",pic5);

	ID = 23;
	UINT8 * pic6 = new UINT8[len];
	writeInt32(pic6,ID);
	int i32test = addFrame( TFHandle,pic6);
	if(i32test == 0)
	{
		printf("pic6 = %x,32test = %d\n",pic6,i32test);
	}
	
	ID = 23;
	UINT8 * pic7 = new UINT8[len];
	writeInt32(pic7,ID);
	if(0 == addFrame( TFHandle,pic7))
		printf("pic7 = %x\n",pic7);

	printf("\ntest  addFrame ok\n");
	/*******/
}

void Driver02_10_79::writeInt32(UINT8 * data, int value)
{
	data[0] =  (char)(value & 0xff);
	data[1] =  (char)((value >> 8) & 0xff);
	data[2] =  (char)((value >> 16) & 0xff);
	data[3] =  (char)((value >> 24) & 0xff);
	
}

	 
	

