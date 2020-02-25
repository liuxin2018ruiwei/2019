/********************************************************************
* Name      :  LeftAreaCaptureStratege.cpp                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  liuxin                                        
* Date      :  2018-06-25                                                 
********************************************************************/
#include <stdio.h>
#include "LeftAreaCaptureStratege.h"
#include <time.h>
#include <sys/time.h>
#include "DetectCfgInfo.h"

#include <FaceDataManage.h>
#include "EncodeModule.h"


int LeftAreaCaptureStratege::init(void *priData )
{
#ifdef DEBUG_LOG
	printf("%d : %s\n", __LINE__, __func__);
#endif	
	
	if( nullptr == priData )
	{
		printf("ERR: %d %s priData=%P\n", __LINE__, __func__, priData );
		
		return -1;
	}
	return 0;
}


int LeftAreaCaptureStratege::captureFace(FaceNode *pFaceNode)
{
	//printf("LeftAreaCaptureStratege :captureFace\n");
	struct timeval tv;
	int ret32 = -1;
	gettimeofday(&tv,NULL);
	//printf("[LX] set time %d:%d\n",tv.tv_sec,tv.tv_usec);
	long long nowTime =  (long long)tv.tv_sec*1000 +  (long long)(tv.tv_usec/1000);
	long long refreshTime = pFaceNode->RefreshTime;
	//printf("nowTime = %lld, refreshTime = %lld, (nowTime - refreshTime) = %lld ,pFaceNode -> trackID = %lld \n",
	//	nowTime,refreshTime, (nowTime - refreshTime), pFaceNode->TrackNo);
	int intervalTime = (int)(nowTime - refreshTime);	
	if(( intervalTime > TIME_INTERVAL_LETF_CAPTURE ) &&(0 == pFaceNode->SelectTime ))
	{
		//1.sent to Code Modle
		ret32 = EncodeModule::getInstance()->AddFaceEncodeQue(pFaceNode);
		pFaceNode->SelectTime = nowTime;
		//printf("in capture left\n\n");
		//2.delete faceNode//
		//printf("nowTime - refreshTime > 1.8,remove Data : %d\n",pTreeNode->faceData.TrackNo);
		//printf(" root = %x; root->faceData.TrackNo = %d,Left = %x.right = %x\n",root,root->faceData.TrackNo,root->left,root->right);
		return ret32;
	}

	return -1;
}


LeftAreaCaptureStratege::~LeftAreaCaptureStratege()
{
}


LeftAreaCaptureStratege::LeftAreaCaptureStratege()
{
}
