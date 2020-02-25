/********************************************************************
* Name      :  DetectCfgInfo.cpp                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :  获取或者设置DetectCfg这个结构体变量（除了心跳和掩码）                                                   
* Author    :  liuxin                                        
* Date      :  2018-06-25                                                 
********************************************************************/
#include <stdio.h>
#include "DetectCfgInfo.h"


DetectCfgInfo* DetectCfgInfo::m_this =  nullptr;


UINT8 DetectCfgInfo::getDetectType()
{
	return m_detectType;
}
void DetectCfgInfo::setDetectType(UINT8 value)
{
	m_detectType = value;
}
  
UINT8 DetectCfgInfo::getDetectThreshold()
{
	return m_detectThreshold;
}
void DetectCfgInfo::setDetectThreshold(UINT8 value)
{
	m_detectThreshold = value;
}

  
UINT8 DetectCfgInfo::getBackGroundCtrl()
{
	return m_backgroundCtrl;
}
void DetectCfgInfo::setBackGroundCtrl(UINT8 value)
{
	m_backgroundCtrl = value;
	m_backGroundSuperstack = (m_backgroundCtrl>>4)&0x0f;
	m_backGrCompLevel = m_backgroundCtrl &0x0f;
}

UINT8 DetectCfgInfo::getFaceCtrl()
{
	return m_faceCtrl;
}
void DetectCfgInfo::setFaceCtrl(UINT8 value)
{
	m_faceCtrl = value;
	m_faceType = ( m_faceCtrl >> 4 ) & 0x0f;
	m_faceCompressLevel = m_faceCtrl & 0x0f;
}

UINT8 DetectCfgInfo::getDetectTime()
{
	return m_detectTime;
}
void DetectCfgInfo::setDetectTime(UINT8 value)
{
	m_detectTime= value;
}
  
UINT16 DetectCfgInfo::getFaceSize()
{
	return m_faceSize;
}
 void DetectCfgInfo::setFaceSize(UINT16 value)
 {
	 m_faceSize = value;
 }

UINT8 DetectCfgInfo::getFaceType()
{
	return m_faceType;
}
UINT8 DetectCfgInfo::getFaceCompressLevel()
{
	return m_faceCompressLevel;
}
UINT8 DetectCfgInfo::getBackGrundCompressLevel()
{
	return m_backGrCompLevel;
}
UINT8 DetectCfgInfo::getBackGrundSuperstack()
{
	return m_backGroundSuperstack;
}
DetectCfgInfo* DetectCfgInfo::getInstance()
{
	if( nullptr == m_this )
	{
		m_this = new DetectCfgInfo;
	}

	return m_this;
}

int DetectCfgInfo::initDetectCfgInfo(void *priData )
{
#ifdef DEBUG_LOG
	printf("%d : %s\n", __LINE__, __func__);
#endif	
	
	if( nullptr == priData )
	{
		printf("ERR: %d %s priData=%P\n", __LINE__, __func__, priData );
		
		return -1;
	}
	m_detectType = 1;
	m_detectThreshold = 1;
	m_detectTime = DEFUTE_DETECT_TIME;
	//m_backgroundCtrl = 0x00;
	setBackGroundCtrl(0x40);
	//m_faceCtrl = 0x00;
	setFaceCtrl(0x14);
	m_faceSize = 0;
	return 0;

}

DetectCfgInfo::~DetectCfgInfo()
{
}


DetectCfgInfo::DetectCfgInfo()
{
}
