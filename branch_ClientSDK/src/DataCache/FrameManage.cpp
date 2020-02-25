/********************************************************************
* Name      :  FrameManage.cpp                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  Zhang Botong                                          
* Date      :  2018-06-08                                                 
********************************************************************/

#include<FrameManage.h>

//#include <stdlib.h>
#include <stdio.h>
#include<cstring>

#include "DetectFaceIF.h"
#include "LittleEndianIO.h"



FrameManage* FrameManage::m_this = nullptr;

int FrameManage::addFrame( UINT8 *pImage )
{
	if( (nullptr == pImage)|| (nullptr == m_pInitImgInfo))
	{
		printf("ERR: %d %s  pImage = %x\n", __LINE__, __func__, pImage );
		return -1;
	}

	UINT32 width = m_pInitImgInfo->Width;
	UINT32 height = m_pInitImgInfo->Height;
	int picLen = width * height *1.5;

	if((0 >= width) || (0 >= height))
	{
		printf("ERR: %d %s pFrameInfo->Width=%d;pFrameInfo->Height = %d\n", __LINE__, __func__, m_pInitImgInfo->Width,m_pInitImgInfo->Height);
		return -1;
	}
	
	
	BackGruondPic * lastData = m_pImagList.back();
	//int imageIdLow = LittleEndianIO::readInt32((const char*)pImage);
	//前面12个是帧头，具体规则是前：
	//byte[0~3]是0xaa
	//bytes[4~7]是一个小端序的32bit值
	//byte[8~11]是0x55
	
	int flag = 0;
	UINT8 * p = pImage;
	for(int i = 0; i < 4; i++)
	{
		if( 0xaa != (*(p+i)) )
		{
			flag = 1;
			break;
		}
		if( 0x55 != *(p + 8 + i) )
		{
			flag = 1;
			break;
		}
	}

	if(flag == 1)
	{
		printf("ERR : pImage byte[0~3] must 0xaa\n");
		return -1;
	}
	
	 p =  pImage + 4;
	//UINT64 imageIdHi = (UINT64)LittleEndianIO::readInt32((const char*)(pImage+4));
	UINT32 imageId = LittleEndianIO::readInt32((const char*)p);//(imageIdHi<<32) |imageIdLow;
	UINT64 lastId = lastData->ID;
	
	if(imageId == lastId)
	{
		printf("[LX] img is in list!pImage:ImgID = %d\n",imageId);
		return -1;
	}

	if(m_pImagCacheList.size() > 0)
	{
	
		BackPicListIter item = m_pImagCacheList.begin();
		BackGruondPic * addData = *item;//获取的是一块空间指针
		
		addData->ID = imageId;
		memcpy( addData->pic, pImage, picLen );

		
		m_pImagCacheList.pop_front();
		//printf("USER: addData = %x ; addData->ID = %x; addData->pic = %x,\n", addData,addData->ID,addData->pic);
		
		m_pImagList.push_back(addData);
		//printf("m_pImagCacheList.size() = %d,m_pImagList.size() = %d\n",m_pImagCacheList.size(),m_pImagList.size());

		//BackPicListIter itemTest = m_pImagList.begin();
		BackGruondPic * getP = m_pImagList.back();//*itemTest;
		//printf("Id = %x, pic = %x \n",getP->ID,getP->pic);

		
	}

	if(m_pImagList.size() > MAX_BACKG_GROUNF_QUEUE_SIZE)//m_pImagList.size() = MAX_BACKG_GROUNF_QUEUE_SIZE+1£»
	{
		BackPicListIter item = m_pImagList.begin();
		BackGruondPic * start = *item ;//mylist.front
		//printf("start = %x;erase ID = %d,(start)->pic = %x\n",start,(start)->ID,(start)->pic);
		m_pImagList.pop_front();
		m_pImagCacheList.push_back( start );
		
	}

	//delete addData;
	return 0;
}

UINT8* FrameManage::getImage(UINT64 frameNumber)//frameId
{
	for(BackPicListIter it = m_pImagList.begin();it != m_pImagList.end(); it++)
	{
	//	printf("it = %x, *it = %x\n",it,*it);
		BackGruondPic * getP = *it;
	//	printf("\n\ngetImage frameNumber = %llx,getP->ID = %lx,getP->pic = %lx\n\n",frameNumber,getP->ID, getP->pic);
		if(frameNumber == getP->ID)
		{
			return (*it)->pic;
		}
	}
	return  NULL;
}





InitImgInfo* FrameManage::getFrameInfo()
{
	return m_pInitImgInfo;
}


int FrameManage::initFrameMange(RWImgInfo * pFrameInfo )
{

	if(nullptr == pFrameInfo)
	{
		printf("ERR: %d %s pFrameInfo=%x\n", __LINE__, __func__, pFrameInfo );
		return -1;
	}
	UINT32 width = pFrameInfo->Width;
	UINT32 height = pFrameInfo->Height;

	if((0 >= width) || (0 >= height))
	{
		printf("ERR: %d %s pFrameInfo->Width=%d;pFrameInfo->Height = %d\n", __LINE__, __func__, pFrameInfo->Width,pFrameInfo->Height);
		return -1;
	}

	if(nullptr == m_pInitImgInfo)
	{
		m_pInitImgInfo = new InitImgInfo;
		m_pInitImgInfo->Height = pFrameInfo->Height;
		m_pInitImgInfo->Width = pFrameInfo->Width;
		m_pInitImgInfo->ImgType = pFrameInfo->ImgType;
		m_pInitImgInfo->ImgLen = pFrameInfo->ImgLen;
		//printf("[LX] **m_pInitImgInfo = %x,Height = %d,m_pInitImgInfo->Width = %d\n",m_pInitImgInfo,m_pInitImgInfo->Height,m_pInitImgInfo->Width);
			
		//return 0;
	}

	//init m_pImagCacheList 
	//printf(" init m_pImagList.size() = %d \n",m_pImagList.size());
	//printf("init m_pImagCacheList.size() =%d\n",m_pImagCacheList.size());
	if( (0 == m_pImagCacheList.size()) && ( 0 == m_pImagList.size() ) )
	{
		m_pImagCacheList.clear();
		m_pImagList.clear();
		int len = m_pInitImgInfo->Height  * m_pInitImgInfo->Width * 1.5;
		for(int i = 0; i < MAX_BACKG_GROUNF_QUEUE_SIZE + 1; i++)
		{
			BackGruondPic *addData = new BackGruondPic;
			addData->ID = i;
			UINT8 * getImg = new UINT8[len];
			addData->pic = getImg;
			//printf("init data is addData = %x; add->pic = %x ,i = %d\n", addData,addData->pic,i);
			m_pImagCacheList.push_back(addData);//初始化是空间指针存入
		}
	}
	
	return 0;
	
}


int FrameManage::uninitFrameMange()
{
	releaseList();
	return 0;
}


FrameManage* FrameManage::getInstance()
{
	if(nullptr == m_this)
	{
		m_this = new FrameManage;
		
	}
	return m_this;
}


FrameManage::FrameManage( )
{
	
}
int FrameManage::releaseList()
{
	for(BackPicListIter it = m_pImagList.begin();it != m_pImagList.end(); )
	{
		delete[] (*it)->pic;
		delete[] *it;
		it = m_pImagList.erase(it);
	}

	for(BackPicListIter it = m_pImagCacheList.begin();it != m_pImagCacheList.end(); )
	{
		delete[] (*it)->pic;
		delete[] *it;
		it = m_pImagCacheList.erase(it);
	}
}



FrameManage::~FrameManage()
{
	if(m_pImagList.size() > 0)
	{
		releaseList();
	}
}

