/********************************************************************
* Name      :  ImgName.cpp                                  
* Describ   : 
* Author    :  liuxin                                         
* Date      :  2018-07-24                                                 
********************************************************************/

#include <stdio.h>
#include "ImgName.h"
#include "ConfigManage.h"
#include <sys/time.h>
#include <time.h>
#include <ctime>

#include <iostream>
#include <string> 
#include<string.h>


ImgName * ImgName::m_this = nullptr;

ImgName * ImgName::getInstance()
{
	if( nullptr == m_this )
	{
		m_this = new ImgName;
	}

	return m_this;
}

int ImgName::initImgName( void *priData )
{
#ifdef DEBUG_LOG
	printf("%d : %s\n", __LINE__, __func__);
#endif
	if( nullptr == priData )
	{
		printf("ERR %d %s pConfig=%P\n", __LINE__, __func__, priData );
		return -1;
	}

	return 0;

}

/*
struct tm
{
    int tm_sec;  
    int tm_min;  
    int tm_hour; 
    int tm_mday;
    int tm_mon;  
    int tm_year;  
    int tm_wday; 
    int tm_yday; 
    int tm_isdst; 
};

*/

int  ImgName::getImagName( char *outName, int *outLen, int inputLen, FaceNode * faceNode, ImageNameType flag )
{
	if( (NULL == outName) || (inputLen <= 0) )
	{
		return -1;
	}

	UINT64 tempCameraID = ConfigManage::getInstance()->getCameraID();
	UINT64 tempTrackID = faceNode->TrackNo;
	UINT64 faceTime = faceNode->InsertTime;
	//printf("time = %u \n",faceNode->InsertTime);

	int ms =  faceNode->InsertTime%1000;

	time_t getTime = faceNode->InsertTime/1000;
	struct tm* timeStruct;
	timeStruct  =  localtime(&getTime);

	int year=  timeStruct->tm_year+1900;
	int mon =  timeStruct->tm_mon+1;
	int day =  timeStruct->tm_mday;
	int hou =  timeStruct->tm_hour;
	int min =  timeStruct->tm_min;
	int sec =  timeStruct->tm_sec;

	char ImagName[256] = {0};
	if(FACE_IMAGE_NAME  == flag)
	{
		sprintf(ImagName, "FACE_%u_%u_",tempCameraID,tempTrackID);  
	}
	else
	{
		sprintf(ImagName, "TG_%d_%u_",tempCameraID,tempTrackID);  
	}
	sprintf(ImagName+ strlen(ImagName), "%04d%02d%02d", year, mon, day);  
	sprintf(ImagName+ strlen(ImagName), "%02d%02d%02d%03d.jpg",hou, min, sec, ms); 
	int imagLen = strlen(ImagName);
	if(inputLen < imagLen+1 )
	{
		printf("input len is too litter.\n");
		return -1;
	}
	*outLen = imagLen+1; // '\0'
	memcpy(outName,ImagName,*outLen);
	//printf("\n%s \n",ImagName);

	return 0;
}

////Face_相机id_跟踪id_时间戳.jpg
int  ImgName::getFaceImagName( char *outName, int *outLen,int inputLen, FaceNode * faceNode )
{
	return getImagName( outName, outLen,inputLen, faceNode, FACE_IMAGE_NAME);
}


//TG_相机id_跟踪id_时间戳.jpg
int  ImgName::getBackgroundImgName(char * outName, int *outLen,int inputLen, FaceNode * faceNode)
{
	return getImagName( outName, outLen, inputLen, faceNode ,BACKGROUND_IMG_NAME);
}



ImgName::~ImgName()
{
}

ImgName::ImgName()
{
}
