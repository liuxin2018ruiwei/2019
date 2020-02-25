/********************************************************************
* Name      :  FaceDataManage.cpp                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  liuxin                                       
* Date      :  2018-06-08                                                 
********************************************************************/

#include <FaceDataManage.h>
#include <cstring>
#include <stdio.h>
#include "FaceDetector.h"
#include "BaseCaptureStratege.h"
#include <time.h>
#include <sys/time.h>
#include "RealTimeCaptureStratege.h"
#include "LeftAreaCaptureStratege.h"


FaceDataManage* FaceDataManage::m_this =  nullptr;



int FaceDataManage::findNode(FaceNode *faceNode)
{
	//2分查找
	int len = m_faceNodeVec.size();
	int low = 0;
	int hight = len;
	while(low < hight)
	{
		int mid = (low+hight)/2;
		FaceNode midNode = m_faceNodeVec[mid];

		if(midNode.TrackNo == faceNode->TrackNo)
		{
			//更新找到的节点信息
			midNode.RefreshTime = getNowTimeMS();
			midNode.RectHeight = faceNode->RectHeight;
			midNode.RectWidth = faceNode->RectWidth;
			midNode.RectX = faceNode->RectX;
			midNode.RectY = faceNode->RectY;
			midNode.frameNumber = faceNode->frameNumber;
			midNode.RectType = faceNode->RectType;
			midNode.QScore = faceNode->QScore;
			m_faceNodeVec[mid] = midNode;
			return mid;
		}
		else if(midNode.TrackNo > faceNode->TrackNo)
		{
			hight = mid-1;
		}
		else
		{
			low = mid + 1; 
		}
	}

	//printf("add data :%d\n",faceNode->TrackNo);

	faceNode->InsertTime = getNowTimeMS();
	faceNode->SelectTime = 0;
	m_faceNodeVec.insert(m_faceNodeVec.begin()+low,*faceNode);
	return 0;

}


int FaceDataManage::deleteNode(UINT64 key)
{
	
	int len = m_faceNodeVec.size();
	int low = 0;
	int hight = len;
	int mid = 0;

	while(low <= hight)
	{
		int mid = (low+hight)/2;
		FaceNode midNode = m_faceNodeVec[mid];
		UINT64 midNodeData = midNode.TrackNo;
		if(midNodeData == key)
		{
			//printf("remove data :%d\n",midNodeData);
			//m_faceNodeVec.erase (m_faceNodeVec.begin() + mid );
			return  mid;
		}
		else if(midNodeData > key)
		{
			hight = mid-1;
		}
		else
		{
			low = mid + 1; 
		}
	}

	return -1;

}

/*
void FaceDataManage::pftFaceVec()
{
	printf("size = %d\n",m_faceNodeVec.size());
	int len = m_faceNodeVec.size();
	for(int i = 0; i < len; i++)
	{
		printf("i = %d ,%d ,itemVec = %x\n",i,m_faceNodeVec[i].TrackNo,&(m_faceNodeVec[i]));
	}
	printf("\n");
}
*/
int FaceDataManage::dealALLNode( FaceNode* node )
{
	// 1 .LeftAreaCaptureStratege
	UINT64  nowTime = getNowTimeMS();
	UINT64 refreshTime = (UINT64)(node->RefreshTime);
	//printf("[LX] nowTime = %lld,refreshTime = %lld\n",nowTime,refreshTime);
	if( (nowTime - refreshTime) > TIME_DELETE_CAPTURE  )
	{
		 int eraseLocation =  deleteNode(node->TrackNo);
		 return eraseLocation;
	}
	return -1;
}

int FaceDataManage::refreshAllFace(RWFaceInfo *pExtData)
{
//printf("**test**,m_faceNodeVec.size()= %d,GET_LIST_STRATEGE.size()= %d\n",m_faceNodeVec.size(),GET_LIST_STRATEGE.size());
//	pftFaceVec();
	int i = 0;
	FaceNodeVecItor end = m_faceNodeVec.end();
	for(FaceNodeVecItor itemVec = m_faceNodeVec.begin(); itemVec != end;)
	{
		FaceNode* tempFace = (FaceNode*)&(*itemVec);
		for(StrategeListIter item = GET_LIST_STRATEGE.begin(); item != GET_LIST_STRATEGE.end(); item++)
		{
			(*item)->captureFace(tempFace);
		}

		
		int location =   dealALLNode(tempFace);
		if(location >= 0)
		{
			//printf("\ni = %d :itemVec = %x\n",i++,itemVec);
			itemVec = m_faceNodeVec.erase (m_faceNodeVec.begin() + location );
			//itemVec = m_faceNodeVec.begin();
			end = m_faceNodeVec.end();
		}
		else
		{
			itemVec++;
		}
		
		//pftFaceVec();//test
	}
	return 0;
}


int FaceDataManage::refreshFaceNode(RWFaceInfo *pExtData)
{
	if(pExtData == NULL)
	{
		printf("ERR: pREFaceInfo is NULL\n ");
		return -1;
	}
	
	/// 获取数据
	FaceNode tempNode;
	getFaceNodeByFaceInfo(&tempNode,pExtData);

	updataFaceNode( &tempNode, pExtData );
	findNode(&tempNode);
}


UINT64 FaceDataManage::getNowTimeMS()
{
	struct timeval tv;
	gettimeofday(&tv,NULL);
	UINT64 retTime =  (UINT64)((long long)tv.tv_sec*1000 +  (long long)(tv.tv_usec/1000) );//ms
	return retTime;
}


int FaceDataManage::getFaceNodeByFaceInfo(FaceNode * pFaceData, RWFaceInfo *pRWFaceInfo)
{

/*
	typedef struct _strFaceNode
	{
		UINT64 frameNumber;//图片编号
		UINT64 TrackNo; //跟踪号
		UINT32 RectX; // 人脸框左上角的横坐标
		UINT32 RectY; //人脸框左上角的纵坐标
		UINT32 RectWidth; // 人脸框的宽
		UINT32 RectHeight; // 人脸框的高
		UINT8  RectType; //人脸框类型： 1：头肩框， 2：半身照框，3：全身照框， 4：人脸框
		float  QScore;//图片质量分
		UINT64 InsertTime; //创建时间；
		UINT64 RefreshTime;//更新时间；
		UINT64 SelectTime;//抓拍时间
	}FaceNode;

	//6)矩形框信息接口	  
	typedef struct _strRWFaceInfo 
	{ 
		  UINT64 ImgID; //关联的图片ID
		  UINT64 TrackNo; //跟踪号
		  UINT32 RectX; // 人脸框左上角的横坐标
		  UINT32 RectY; //人脸框左上角的纵坐标
		  UINT32 RectWidth; // 人脸框的宽
		  UINT32 RectHeight; // 人脸框的高
		  UINT8 RectType; //人脸框类型：0：背景图, 1：头肩框, 2：半身照框,3：全身照框, 4：人脸框
		  UINT8 Align[3]; // 对齐,保留,暂不使用
		  float QScore;//图片质量分
	}RWFaceInfo;  
*/
	if((pFaceData == NULL)||(pRWFaceInfo == NULL))
	{
		printf("ERR : updataTreeNode pFaceData or pRWFaceInfo is NULL\n");
		return -1;
	}

	pFaceData->frameNumber = pRWFaceInfo->ImgID;
	pFaceData->TrackNo = pRWFaceInfo->TrackNo;
	pFaceData->RectX = pRWFaceInfo->RectX;
	pFaceData->RectY = pRWFaceInfo->RectY;
	pFaceData->RectWidth = pRWFaceInfo->RectWidth;
	pFaceData->RectHeight = pRWFaceInfo->RectHeight;
	pFaceData->RectType = pRWFaceInfo->RectType;
	pFaceData->QScore = pRWFaceInfo->QScore;
	//pFaceData->InsertTime?

}

int FaceDataManage:: updataFaceNode(FaceNode * pFaceNode, RWFaceInfo *pREFaceInfo)
{
	if((pFaceNode == NULL)||(pREFaceInfo == NULL))
	{
		printf("ERR : updataTreeNode :pFaceNode = %x ,pREFaceInfo = %x\n",pFaceNode,pREFaceInfo);
		return -1;
	}
	getFaceNodeByFaceInfo( pFaceNode, pREFaceInfo);
	pFaceNode->RefreshTime = getNowTimeMS();

	
	//printf("--updataTreeNode  pFaceNode->RefreshTime = %lld;pREFaceInfo->QScore = %f\n",
	//		pFaceNode->RefreshTime,pREFaceInfo->QScore);
	return 0;
}


FaceDataManage* FaceDataManage::getInstance()
{
	if( nullptr == m_this )
	{
		m_this = new FaceDataManage;
	}

	return m_this;
}

vector<FaceNode>& FaceDataManage::getFaceNodeVec()
{
	return m_faceNodeVec;
}

int FaceDataManage::releaseData()
{
	for(FaceNodeVecItor item = m_faceNodeVec.begin(); item != m_faceNodeVec.end(); )
	{
		item = m_faceNodeVec.erase(item);
	}
	//printf("[LX] release data m_faceNodeVec.size() = %d \n",m_faceNodeVec.size());
	return 0;
}

FaceDataManage::FaceDataManage()
{
	
}

int FaceDataManage::initMsgProcessor( void * priData )
{
	if( nullptr == priData )
	{
		printf("ERR: %d %s priData=%P\n", __LINE__, __func__, priData );
		
		return -1;
	}
	
	return 0;
}
void FaceDataManage::uninitMsgProcessor( )
{
	releaseData();
}

FaceDataManage:: ~FaceDataManage()
{
	releaseData();
}


void FaceDataManage( )
{

}
