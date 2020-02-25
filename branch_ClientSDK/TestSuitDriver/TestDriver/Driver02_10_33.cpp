/********************************************************************
* Name      :  Driver02_10_33.cpp                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  Zhang Botong                                          
* Date      :  2018-07-02                                                 
********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "DetectFaceIF.h"
#include "Driver02_10_33.h"


#define	 MASK_DETECT_TYPE 		(0x01)
#define  MASK_DETECT_HEART_BEAT (0x02) 
#define  MASK_DETECT_THRESHOLD 	(0x04)
#define	 MASK_BACK_GROUND_CTRL 	(0x08)
#define	 MASK_BACK_FACE_CTRL 	(0x10)
#define	 MASK_DETECT_TIME		(0x20)
#define	 MASK_FACE_SIZE			(0x40)


Driver02_10_33::Driver02_10_33(unsigned int nCaseId ): Driver( nCaseId )
{

}

int Driver02_10_33::init()
{
	Driver::init();
}

int Driver02_10_33::run()
{
	printf("Driver02_10_33 [test] run \n");
	LXTestWBS_02_10_33();
	
	return 0;
}

int Driver02_10_33::uninit()
{
	Driver::uninit();
}

Driver02_10_33::~Driver02_10_33()
{
	
}


void Driver02_10_33::LXTestWBS_02_10_33()
{
	printf("---test1 ---- LXTest: WBS_02_10_33 ~ WBS_02_10_35 -----\n");
	void *TFHandle = getHandle();

	int nCount = 2;
	Point tempPoint[3];
	tempPoint[0].X = 20;	
	tempPoint[0].Y = 20;	
	tempPoint[1].X = 40;	
	tempPoint[1].Y = 20;
	printf("LXTestWBS_02_10_19 nCount = %d",nCount);
	addDetectRegion( TFHandle, (Point *)(&tempPoint),  nCount );

	nCount = 9;	
	Point tempPoint2[9];
	tempPoint2[0].X = 50;	
	tempPoint2[0].Y = 20;	
	tempPoint2[1].X = 100;	
	tempPoint2[1].Y = 20;	
	tempPoint2[2].X = 150;	
	tempPoint2[2].Y = 50;	
	tempPoint2[3].X = 100;	
	tempPoint2[3].Y = 100;	
	tempPoint2[4].X = 50;	
	tempPoint2[4].Y = 110;	
	tempPoint2[5].X = 30;	
	tempPoint2[5].Y = 100;	
	tempPoint2[6].X = 0;	
	tempPoint2[6].Y = 50;	
	tempPoint2[7].X = 30;	
	tempPoint2[7].Y = 30;	
	tempPoint2[7].X = 20;	
	tempPoint2[7].Y = 20;
	printf("LXTestWBS_02_10_19 nCount = %d",nCount);
	addDetectRegion( TFHandle, (Point *)(&tempPoint2),  nCount );
	printf("---end ---- LXTestWBS_02_10_33  -----\n");

	
	nCount = 3;
	tempPoint[0].X = 20;	
	tempPoint[0].Y = 20;	
	tempPoint[1].X = 40;	
	tempPoint[1].Y = 20;
	tempPoint[2].X = 20;	
	tempPoint[2].Y = 40;
	printf("LXTestWBS_02_10_19 nCount = %d",nCount);
	addDetectRegion( TFHandle, (Point *)(&tempPoint),  nCount );
	addShieldRegion( TFHandle, (Point *)(&tempPoint),  nCount );

}

