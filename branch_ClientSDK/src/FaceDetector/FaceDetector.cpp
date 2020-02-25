/********************************************************************
* Name      :  FaceDetector.cpp                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  Zhang Botong                                          
* Date      :  2018-06-06                                                 
********************************************************************/
#include <stdio.h>
#include "FaceDetector.h"
#include "HeartBeatInfo.h"
#include "DetectCfgInfo.h"
#include "RealTimeCaptureStratege.h"
#include "LeftAreaCaptureStratege.h"
#include <FaceDataManage.h>
#include "SpaceCaptureStratege.h"
#include "EncodeModule.h"
#include "CountInterface.h"


FaceDetector* FaceDetector::m_this =  nullptr;

FaceDetector* FaceDetector::getInstance()
{
	if( nullptr == m_this )
	{
		m_this = new FaceDetector;
	}

	return m_this;
}

int FaceDetector::initFaceDetector(void *priData )
{
	if( nullptr == priData )
	{
		printf("ERR: %d %s priData=%P\n", __LINE__, __func__, priData );
		
		return -1;
	}
	int tempData = 1;
	int ret32 = 0;
	ret32 |=HeartBeatInfo::getInstance()->initHeartBeatInfo((void *)&tempData);
	ret32 |=DetectCfgInfo::getInstance()->initDetectCfgInfo((void *)&tempData);
	ret32 |=CountInterface::getInstance()->resetCounterInfo();
	ret32 |= EncodeModule::getInstance()->initEncodeModule((void *)&tempData);
	m_flagRecoverBarudrate = 0;
	return ret32;

}

void FaceDetector::uninitFaceDetector()
{
	CountInterface::getInstance()->releaseData();
	EncodeModule::getInstance()->uninitEncodeModule();
	releaseData();
}

int FaceDetector::addDetectRegion( Point *pPoint, int nCount )
{

	if((pPoint == NULL)||(nCount > MAX_POINT_NUM)||(nCount < MIN_POINT_NUM) )
	{
		printf("ERR: pPiont = %x,nCount = %d\n",pPoint,nCount);
		return -1;
	}
	if(m_listShieldRegion.size() > 0)
	{
		//printf("[LX] m_listShieldRegion.size() = %d\n",m_listShieldRegion.size());
		return -1;
	}
	RegionJudger addRegion;
	int ret =  addRegion.setRegion( pPoint,nCount);
	if(ret != 0)
	{
		return -1;
	}

	//add new region
	if(m_listRegion.size() < DetectionRegionMax)
	{
		m_listRegion.push_back(addRegion);
	}
	else
	{
		printf("add list is full ,please reslease some zone. \n");
		return -1;
	}

	return 0;
}

int FaceDetector::deletetDetectRegion( Point *pPoint, int nCount )
{
	//printf("[lx]--------------nCount = %d--------------------\n",nCount);
	if((pPoint == NULL)||(nCount > MAX_POINT_NUM)||(nCount < MIN_POINT_NUM) )
	{
		printf("ERR: pPiont = %x,nCount = %d\n",pPoint,nCount);
		return -1;
	}
	int flagEqual = 0;
	RegionJudger::Region tempRegion;
	Point *pTemp;
	for(RegionListIter item = m_listRegion.begin(); item != m_listRegion.end();item++)
	{
		item->getRegion(tempRegion);
		//printf("tempRegion.Sum = %d ; nCount = %d\n",tempRegion.Sum,nCount);
		if(tempRegion.Sum != nCount)
		{
			continue;
		}
		flagEqual = 0;
		for(int i = 0; i < nCount; i++)
		{
			pTemp = pPoint + i;
			if( (tempRegion.Vertex[i].X != pTemp->X) || (tempRegion.Vertex[i].Y != pTemp->Y) )
			{
				flagEqual = 1;
				break;
			}
		}
		if(flagEqual == 0)
		{
			item = m_listRegion.erase(item);
			//printf("deletetDetectRegion ok \n");
			return 0;
		}
		
	}

	printf("not find  DetectRegion.\n");
	return -1;
}


int FaceDetector::addShieldRegion( Point *pPoint, int nCount )
{
	if((pPoint == NULL)||(MAX_POINT_NUM < nCount)||( MIN_POINT_NUM > nCount) )
	{
		printf("ERR: pPiont = %x,nCount = %d\n",pPoint,nCount);
		return -1;
	}

	RegionJudger addShieldRegion;
	int ret =  addShieldRegion.setRegion( pPoint,nCount);
	if(ret != 0)
	{
		return -1;
	}

	//add new addShieldRegion
	if(m_listShieldRegion.size() < DetectionShieldRegionMax)
	{	
		m_listShieldRegion.push_back(addShieldRegion);
	}
	else
	{
		printf("ShieldRegion list is full, please relase old data\n");
		return -1;
	}
	return 0;
	
}
int FaceDetector::deletetShieldRegion (  Point *pPoint, int nCount )
{
	if((pPoint == NULL)||(nCount > MAX_POINT_NUM)||(nCount < MIN_POINT_NUM) )
	{
		printf("ERR: pPiont = %x,nCount = %d\n",pPoint,nCount);
		return -1;
	}
	int flagEqual = 0;
	RegionJudger::Region tempRegion;
	Point *pTemp;
	for(RegionListIter item = m_listShieldRegion.begin(); item != m_listShieldRegion.end();item++)
	{
		item->getRegion(tempRegion);
		if(tempRegion.Sum == nCount)
		{
			flagEqual = 0;
			for(int i = 0; i < nCount; i++)
			{
				pTemp = pPoint + i;
				if( (tempRegion.Vertex[i].X != pTemp->X) || (tempRegion.Vertex[i].Y != pTemp->Y) )
				{
					flagEqual = 1;
					break;
				}
			}
			if(flagEqual == 0)
			{ 	

				item = m_listShieldRegion.erase(item);
				return 0;
			}
		}
	}

	printf("not find point\n");
	return -1;
}




int FaceDetector::CaptureThresholdFilter(RWFaceInfo *pREFaceInfo)
{
	UINT8 captureThreshold = DetectCfgInfo::getInstance()->getDetectThreshold();// get Capture threshold
	//printf("captureThreshold = %d\n",captureThreshold);
	if( 0 == captureThreshold)
	{
		return 0;
	}
	else if( 0  < captureThreshold)
	{
		if( (pREFaceInfo->QScore > captureThreshold)
			&&(FACE_FRAME == pREFaceInfo->RectType) )
		{
			return 0;
		}
	}
	
	return -1;
}

int FaceDetector::FaceSizeFilter(RWFaceInfo *pExtData)
{
	UINT32 getW = pExtData->RectWidth;
	UINT32 getFSize = (UINT32)DetectCfgInfo::getInstance()->getFaceSize();
	if(getW > getFSize)
	{
		return 0;
	}
	return -1;
}

void FaceDetector::setDetectType(UINT8 value)
{
	UINT8 detectType = value;
	if(  CAMERA_LEAVE_SNAP == detectType )
	{
		//clean old CaptureStratege
		for(StrategeListIter item = m_pListStratege.begin(); item != m_pListStratege.end();)
		{
			delete (*item);
			item = m_pListStratege.erase(item);
		}
	
		BaseCaptureStratege * base = new LeftAreaCaptureStratege;
		m_pListStratege.push_back(base);//save && BaseCaptureStratege
	}
	else if( CAMERA_NOW_SNAP == detectType)
	{
		for(StrategeListIter item = m_pListStratege.begin(); item != m_pListStratege.end();)
		{
			delete (*item);
			item = m_pListStratege.erase(item);
		}
	
		BaseCaptureStratege * base = new RealTimeCaptureStratege;
		m_pListStratege.push_back(base);
	}
	else if( CAMERA_INTERVAL_SNAP == detectType)
	{
		for(StrategeListIter item = m_pListStratege.begin(); item != m_pListStratege.end();)
		{
			delete (*item);
			item = m_pListStratege.erase(item);
		}
	
		BaseCaptureStratege * base = new SpaceCaptureStratege;
		m_pListStratege.push_back(base);
	}
	
	DetectCfgInfo::getInstance()->setDetectType(detectType);
}


int FaceDetector::scaleRect(RWFaceInfo *pSrc, RWFaceInfo *pDst )
{
	Rect  destRect,srcRect;
	destRect.left = pDst->RectX;
	destRect.right = pDst->RectX + pDst->RectWidth;
	destRect.top = pDst->RectY;
	destRect.bottom = pDst->RectY + pDst->RectHeight;

	srcRect.left = pSrc->RectX;
	srcRect.right = pSrc->RectX + pSrc->RectWidth;
	srcRect.top = pSrc->RectY;
	srcRect.bottom = pSrc->RectY+pSrc->RectHeight;
	
	FaceRectType dstType = (FaceRectType)pDst->RectType;
	FaceRectType srcType = (FaceRectType)pSrc->RectType;
	
	int ret32 = EncodeModule::getInstance()->scaleRect(&destRect, &srcRect, dstType, dstType);
	return ret32;
}

int FaceDetector::inDetectRegion(Point point)
{
	RegionJudger::Region tempRegion;
	bool ret = HAS_NOT_FIND_DETECTOR_REGION;
	//Point tempPoint = point;
	if(m_listRegion.size() > 0)
	{
		for(RegionListIter item = m_listRegion.begin(); item != m_listRegion.end();item++)
		{
			item->getRegion(tempRegion);
		 	ret = item->isInRegion(tempRegion,point);
			if(ret)
			{
				return HAS_FIND_DETECTOR_REGION;
			}
		}
		return HAS_NOT_FIND_DETECTOR_REGION;
	}
	

	if(m_listShieldRegion.size() > 0)
	{
		for(RegionListIter item = m_listShieldRegion.begin(); item != m_listShieldRegion.end(); item++)
		{
			item->getRegion(tempRegion);
			ret = item->isInRegion(tempRegion,point);
			if(ret)
			{
				return HAS_FIND_SHILED_REGION;
			}
		}
		return HAS_NOT_FIND_SHILED_REGION;
	}
	return -1;
}


int FaceDetector::filterFace(RWFaceInfo *pExtData)
{
	UINT16 centX,centY;
	Point point;
	RWFaceInfo * tempRWFaceInfo = pExtData;
	centX = tempRWFaceInfo->RectX + tempRWFaceInfo->RectWidth/2;
	centY = tempRWFaceInfo->RectY + tempRWFaceInfo->RectHeight/2;
	point.X = centX;
	point.Y = centY;

	//filter rule 1:the point is in regin
	int ret = inDetectRegion(point);
	if(1 != ret)
	{
		printf("ERR filterFace : the point TrackNo = %lld is not in detect region\n\n",pExtData->TrackNo);
		return -1;
	}

	//filter rule 2:RectType and QScore
	ret = CaptureThresholdFilter(tempRWFaceInfo);
	if(0 != ret)
	{
		printf("ERR filterFace : CaptureThresholdFilter not right\n");
		return -1;
	}
	
	//filter rule 3:faceSize
	ret =  FaceSizeFilter(tempRWFaceInfo);
	if(0 != ret)
	{
		printf("ERR filterFace : FaceSizeFilter not right\n");
		return -1;
	}
	//printf("rule 3:FaceSizeFilter ok\n ");
	return 0;
	
}

int  FaceDetector::selectFace(RWFaceInfo *pExtData)
{
	FaceDataManage::getInstance()->refreshAllFace(pExtData);
	return 0;
}

RegionList & FaceDetector:: getlistRegion()
{
	return m_listRegion;
}

RegionList & FaceDetector:: getListShieldRegion()
{
	return m_listShieldRegion;
}

StrategeList &  FaceDetector:: getListStratege()
{
	return m_pListStratege;
}
int FaceDetector::releaseData()
{

	for(RegionListIter item = m_listShieldRegion.begin(); item != m_listShieldRegion.end();)
	{
		item = m_listShieldRegion.erase(item);
	}

	for(RegionListIter item = m_listRegion.begin(); item != m_listRegion.end();)
	{
		item = m_listRegion.erase(item);
	}

	for(StrategeListIter item = m_pListStratege.begin(); item != m_pListStratege.end();)
	{
		delete (*item);
		item = m_pListStratege.erase(item);
	}
}


FaceDetector::~FaceDetector()
{
	releaseData();
}

int FaceDetector::countPeople(RWFaceInfo *pExtData)
{
	CountInterface::TrackPoint startPoint;
	startPoint.TrackPoint.X =  pExtData->RectX + pExtData->RectWidth / 2;
	startPoint.TrackPoint.Y =  pExtData->RectY + pExtData->RectHeight / 2;
	startPoint.TrackId = pExtData->TrackNo;
	CountInterface::getInstance()->setTrackPoint(startPoint );
}

char *  FaceDetector:: getUpdateFileName()
{
	return m_updateFileName;
}

int  FaceDetector::getFlagRecoverBarudrate()
{
	return m_flagRecoverBarudrate;
}
void FaceDetector:: setflagRecoverBarudrate(int data)
{
	m_flagRecoverBarudrate = data;
}


long long FaceDetector:: getRecoverBarudTime()
{
	return m_RecoverBarudTime ;
}
void FaceDetector:: setRecoverBarudTime(long long time)
{
	m_RecoverBarudTime = time;
}




FaceDetector::FaceDetector()
{
}
