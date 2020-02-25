/********************************************************************
* Name      :  RealTimeCaptureStratege.cpp                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                      
* Author    :  liuxin                                        
* Date      :  2018-06-25                                                 
********************************************************************/
#include <stdio.h>
#include "RealTimeCaptureStratege.h"
#include <time.h>
#include <sys/time.h>
#include "DetectCfgInfo.h"
#include "EncodeModule.h"




int RealTimeCaptureStratege::init(void *priData )
{
	if( nullptr == priData )
	{
		printf("ERR: %d %s priData=%P\n", __LINE__, __func__, priData );
		
		return -1;
	}
	return 0;
}


int RealTimeCaptureStratege::captureFace(FaceNode *pFaceNode)
{
	//printf("RealTimeCaptureStratege: pTreeNode.trackID = %llx\n",pFaceNode->TrackNo);
	struct timeval tv;
	int ret32 = -1;
	gettimeofday(&tv,NULL);
	//printf("[LX] set time %d:%d\n",tv.tv_sec,tv.tv_usec);
	long long nowTime =  (long long)tv.tv_sec*1000 +  (long long)(tv.tv_usec/1000);
	long long refreshTime = pFaceNode->RefreshTime;
	long long insertTime = pFaceNode->InsertTime;

	long long captureTimeInterval = (long long)DetectCfgInfo::getInstance()->getDetectTime();
	UINT64 flagHasCapture =  pFaceNode->SelectTime; 
	//printf("nowTime = %lld, insertTime = %lld, captureTimeInterval = %lld,  flagHasCapture = %lld pFaceNode = %lld,pFaceNode = %x\n",
	//			nowTime,insertTime,captureTimeInterval,flagHasCapture, pFaceNode->TrackNo, pFaceNode);
	if( ( (nowTime - insertTime) > captureTimeInterval*1000) && (flagHasCapture == 0) )
	{
		//sent to Code Modle
		ret32 = EncodeModule::getInstance()->AddFaceEncodeQue(pFaceNode);
		//updata SelectTime
		pFaceNode->SelectTime = nowTime;
		return ret32;
	}

	return -1;
}


RealTimeCaptureStratege::~RealTimeCaptureStratege()
{
}


RealTimeCaptureStratege::RealTimeCaptureStratege()
{
}
