/********************************************************************
* Name      :  DataCache.cpp                                  
* Describ   :  the defination of the class
* Author    :  Zhang Botong                                          
* Date      :  2018-06-06                                                 
********************************************************************/

#include <stdio.h>
#include "DataCache.h"
#include "FrameManage.h"
#include "FaceDataManage.h"

DataCache * DataCache::m_this = nullptr;

DataCache * DataCache::getInstance()
{
	if( nullptr == m_this )
	{
		m_this = new DataCache;
	}

	return m_this;
}

int DataCache::initDataCache( RWImgInfo *FrameInfo )
{
#ifdef DEBUG_LOG
	printf("%d : %s\n", __LINE__, __func__);
#endif
	if( nullptr == FrameInfo )
	{
		printf("ERR %d %s pConfig=%P\n", __LINE__, __func__, FrameInfo );
		return -1;
	}
	if( (0 == FrameInfo->Width) || (0 == FrameInfo->Height) )
	{
		printf("ERR:  FrameInfo->Width = %d,FrameInfo->Height = %d\n",FrameInfo->Width, FrameInfo->Height);	
		return -1;
	}
	//printf("FrameInfo = %x,FrameInfo->Height = %d ,FrameInfo->Width = %d\n",FrameInfo,FrameInfo->Height,FrameInfo->Width);
	int	nRet =FrameManage::getInstance()->initFrameMange(FrameInfo);
	//printf("init FrameManage \n");
	
	//nRet = FaceDataManage::getInstance()->initMsgProcessor();
	return nRet;

}

int DataCache::uninitDataCache()
{
	FrameManage::getInstance()->uninitFrameMange();
	//printf("uninitConfigManage	FrameManage  \n");

	FaceDataManage::getInstance()->uninitMsgProcessor(); //faceNode relese
	//printf("uninitConfigManage	FaceDataManage  \n");
}


DataCache::~DataCache()
{
}

DataCache::DataCache()
{
}
