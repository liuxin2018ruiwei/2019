/********************************************************************
* Name      :  SpaceCaptureStratege.cpp                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                      
* Author    :  liuxin                                        
* Date      :  2018-07-9                                                 
********************************************************************/
#include <stdio.h>
#include "SpaceCaptureStratege.h"
#include <time.h>
#include <sys/time.h>
#include "DetectCfgInfo.h"
#include "EncodeModule.h"


int SpaceCaptureStratege::init(void *priData )
{
	if( nullptr == priData )
	{
		printf("ERR: %d %s priData=%P\n", __LINE__, __func__, priData );
		
		return -1;
	}
	return 0;
}


int SpaceCaptureStratege::captureFace(FaceNode *pFaceNode)
{
	//printf("SpaceCaptureStratege: pFaceNode->TrackNo = %llx\n",pFaceNode->TrackNo);
	struct timeval tv;
	int ret32 = -1;
	gettimeofday(&tv,NULL);
	//printf("[LX] set time %d:%d\n",tv.tv_sec,tv.tv_usec);
	long long nowTime =  (long long)tv.tv_sec*1000 +  (long long)(tv.tv_usec/1000);//ms
	long long selectTime = pFaceNode->SelectTime;

	long long captureTimeInterval = (long long)DetectCfgInfo::getInstance()->getDetectTime(); 
	//printf("SpaceCaptureStratege nowTime = %lld,  selectTime = %lld, captureTimeInterval = %lld\n",nowTime, selectTime, captureTimeInterval);
	if( (nowTime - selectTime) > captureTimeInterval *1000  )
	{
		//sent to Code Modle
		ret32 = EncodeModule::getInstance()->AddFaceEncodeQue(pFaceNode);
		//updata selectTime
		pFaceNode->SelectTime = nowTime;
		return ret32;
	}

	return -1;
}


SpaceCaptureStratege::~SpaceCaptureStratege()
{
}


SpaceCaptureStratege::SpaceCaptureStratege()
{
}
