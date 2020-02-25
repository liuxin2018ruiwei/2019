/********************************************************************
* Name      :  Driver02_10_32.cpp                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  Zhang Botong                                          
* Date      :  2018-07-02                                                 
********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "DetectFaceIF.h"
#include "Driver02_10_32.h"


#define	 MASK_DETECT_TYPE 		(0x01)
#define  MASK_DETECT_HEART_BEAT (0x02) 
#define  MASK_DETECT_THRESHOLD 	(0x04)
#define	 MASK_BACK_GROUND_CTRL 	(0x08)
#define	 MASK_BACK_FACE_CTRL 	(0x10)
#define	 MASK_DETECT_TIME		(0x20)
#define	 MASK_FACE_SIZE			(0x40)


Driver02_10_32::Driver02_10_32(unsigned int nCaseId ): Driver( nCaseId )
{

}

int Driver02_10_32::init()
{
	Driver::init();
}

int Driver02_10_32::run()
{
	printf("Driver02_10_32 [test] run \n");
	LXTestWBS_02_10_32();
	
	return 0;
}

int Driver02_10_32::uninit()
{
	Driver::uninit();
}

Driver02_10_32::~Driver02_10_32()
{
	
}


void Driver02_10_32::LXTestWBS_02_10_32()
{
	//printf("---test1 ---- LXTestWBS_02_10_32  -----\n");

	printf("---test1 ---- LXTest: WBS_02_10_27~ WBS_02_10_31 -----\n");
	void *TFHandle;
	int a = 566;
	TFHandle = &a;
	
	int nCount = 3;
	Point tempPoint[3];
	tempPoint[0].X = 20;	
	tempPoint[0].Y = 20;	
	tempPoint[1].X = 40;	
	tempPoint[1].Y = 20;
	tempPoint[2].X = 20;	
	tempPoint[2].Y = 40;
	printf("LXTestWBS_02_10_19 nCount = %d",nCount);
	addDetectRegion( TFHandle, (Point *)(&tempPoint),  nCount );
	
	nCount = 4;	
	Point tempPoint1[4];
	tempPoint1[0].X = 50;	
	tempPoint1[0].Y = 20;	
	tempPoint1[1].X = 100;	
	tempPoint1[1].Y = 20;	
	tempPoint1[2].X = 100;	
	tempPoint1[2].Y = 100;	
	tempPoint1[3].X = 50;	
	tempPoint1[3].Y = 100;	
	printf("LXTestWBS_02_10_19 nCount = %d",nCount);
	addDetectRegion( TFHandle, (Point *)(&tempPoint1),  nCount );

	nCount = 8;	
	Point tempPoint2[8];
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
	printf("LXTestWBS_02_10_19 nCount = %d",nCount);
	addDetectRegion( TFHandle, (Point *)(&tempPoint2),  nCount );

	nCount = 8;	
	Point tempPoint3[8];
	tempPoint3[0].X = 50;	
	tempPoint3[0].Y = 20;	
	tempPoint3[1].X = 80;	
	tempPoint3[1].Y = 20;	
	tempPoint3[2].X = 80;	
	tempPoint3[2].Y = 30;	
	tempPoint3[3].X = 100;	
	tempPoint3[3].Y = 30;	
	tempPoint3[4].X = 100;	
	tempPoint3[4].Y = 20;	
	tempPoint3[5].X = 130;	
	tempPoint3[5].Y = 20;	
	tempPoint3[6].X = 130;	
	tempPoint3[6].Y = 50;	
	tempPoint3[7].X = 50;	
	tempPoint3[7].Y = 50;
	printf("LXTestWBS_02_10_19 nCount = %d",nCount);
	addDetectRegion( TFHandle, (Point *)(&tempPoint3),  nCount );

	nCount = 3;
	Point tempPoint4[3];
	tempPoint4[0].X = 20;	
	tempPoint4[0].Y = 20;	
	tempPoint4[1].X = 40;	
	tempPoint4[1].Y = 20;
	tempPoint4[2].X = 20;	
	tempPoint4[2].Y = 40;
	printf("LXTestWBS_02_10_19 nCount = %d",nCount);
	addDetectRegion( TFHandle, (Point *)(&tempPoint4),  nCount );
	
	deletetDetectRegion( TFHandle, (Point *)(&tempPoint4),  nCount );

	nCount = 8; 
	deletetDetectRegion( TFHandle, (Point *)(&tempPoint3),  nCount );
	nCount = 8;	
	deletetDetectRegion( TFHandle, (Point *)(&tempPoint2),  nCount );
	nCount = 4;	
	deletetDetectRegion( TFHandle, (Point *)(&tempPoint1),  nCount );

	nCount = 3;	
	deletetDetectRegion( TFHandle, (Point *)(&tempPoint),  nCount );

	nCount = 4;	
	addShieldRegion( TFHandle, (Point *)(&tempPoint1),  nCount );

	nCount = 8;	
	addShieldRegion( TFHandle, (Point *)(&tempPoint2),  nCount );

	nCount = 8;	
	deletetShieldRegion( TFHandle, (Point *)(&tempPoint2),  nCount );

	nCount = 4;	
	deletetShieldRegion( TFHandle, (Point *)(&tempPoint1),  nCount );
	
	printf("---test1 ---- LXTestWBS_02_10_32  -----\n");
	
}
 
