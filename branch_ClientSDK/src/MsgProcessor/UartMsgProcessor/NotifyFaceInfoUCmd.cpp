/********************************************************************
* Name      :  NotifyFaceInfoUCmd.cpp                                  
* Describ   :  get the face info to use for ex :  left,top,right,bottom,type,quality,blur,light,confidence
* Author    :  liuxin                                        
* Date      :  2018-06-29                                                 
********************************************************************/
#include <stdio.h>
#include "NotifyFaceInfoUCmd.h"
#include "UartCommandType.h"
#include "LittleEndianIO.h"
#include "ProcessReq.h"
#include "Requestion.h"
#include <stdlib.h>
#include "DetectCfgInfo.h"
#include "CountInterface.h"

#include <sys/time.h>






NotifyFaceInfoUCmd::NotifyFaceInfoUCmd():UartCommand( UartCommandCode::QUERY_FACE_COORDINATE ) 
{
	lastTime = 0;
}


int NotifyFaceInfoUCmd::everyIntervalNotifyUse()
{
	//int everyIntervalNotifyUse();
	struct timeval curTime;
	gettimeofday( &curTime, nullptr );
	int nowTimer = curTime.tv_sec;

	// every 5s to send count people 
	int intervalTime = nowTimer - lastTime;
	if( intervalTime >= COUNT_PEOPLE_TIME)
	{
		lastTime = nowTimer;
		
		CountInterface::CounterInfo info;
		CountInterface::getInstance()->getCounterInfo(info);
		//printf("out = %d, in = %d\n", info.GetOut,info.ComeIn);

		CallbackNotifyFunc getPCountPeopleFun = ProcessReq::getInstance()->getFuncCountPeopl();
		//printf("getPCountPeopleFun = %x\n",getPCountPeopleFun);
		if(NULL == getPCountPeopleFun)
		{
			printf("ERR :CallbackNotifyFunc == NULL\n");
			return -1;
		}
		RWNotifyType pNotifyType = RWNotifyType::NotifyCountFace; 
		UINT8 *pExtData = (UINT8 *)(&info);
		UINT32 pDataLen = sizeof(CountInterface::CounterInfo);

		(*getPCountPeopleFun)( &pNotifyType, pExtData, &pDataLen);
	}
	return 0;
}


int NotifyFaceInfoUCmd::processMsg(Message * pMsg)
{
	if(NULL == pMsg)
	{
		return -1;
	}

	int ret32 = 0;
	UINT32 frameNumber;
	UINT8 detectedObjects;
	UINT16 left,top,right,bottom;
	UINT32 trackID;
	UINT8 type,quality,blur,light,confidence;
	int dataLen = 0;
	
	
	UINT16 commandCode= pMsg->getCode();//Command Code
	UINT8  flags = pMsg->getFlags(); //Flags
	UINT8  sequenceNumber = pMsg->getSequenceNumber(); //Sequence Number
	UINT8* p = (UINT8 *)(pMsg->getData());


	frameNumber = (UINT32)LittleEndianIO::readInt32((const char*)p);
	p = p+4;
	//printf("[LX] frameNuber = %d\n",frameNumber);
	detectedObjects = *p++;
	//printf("[LX] detec frameNumber = %x detectedObjects = %d\n", frameNumber, detectedObjects);
	//malloc
	dataLen = sizeof(RWFaceInfo)*detectedObjects;
	RWFaceInfo * mRWFaceInfo = new  RWFaceInfo[detectedObjects];
	
	for(int i = 0; i < detectedObjects; i++)
	{
		left = (UINT16)LittleEndianIO::readInt16((const char*)p);
		p = p+2;
		top =  (UINT16)LittleEndianIO::readInt16((const char*)p);
		p = p+2;
		right = (UINT16)LittleEndianIO::readInt16((const char*)p);
		p = p+2;
		bottom =  (UINT16)LittleEndianIO::readInt16((const char*)p);
		p = p+2;
		//printf("[ lx ] i = %d; left = %d,top = %d,right = %d,bottom = %d \n",i ,left,top,right,bottom);

		trackID =  (UINT32)LittleEndianIO::readInt32((const char*)p);
		p = p+4;

		type = *p++;
		quality= *p++;
		blur= *p++;
		light= *p++;
		confidence= *p++;
		//printf("[LX ] tarckID = %d;type = %d,quality =%d, blur= %d, light = %d, confidence = %d \n", trackID,type,quality,blur,light,confidence);
		/*
		  typedef void (*CallbackNotifyFunc)( RWNotifyType *pNotifyType, UINT8 *pExtData, UINT32 *pDataLen );	
			CallbackReplyFunc RegisterReplyFunc; //在服务端反馈登录的结果时调用	
			CallbackReplyFunc CancelRelpyFunc; //在服务端反馈注销的结果时调用
			CallbackNotifyFunc NotifyFaceFunc;//在服务端通知图像信息时调用	
			CallbackNotifyFunc NotifyClientFunc;//在服务端通知客户端状态时调用
			
		*/
/*
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
		//init
	    RWFaceInfo *tempRWFaceInfo = &mRWFaceInfo[i];
		tempRWFaceInfo->ImgID =  frameNumber;
		tempRWFaceInfo->TrackNo = trackID;
		tempRWFaceInfo->RectX = left;
		tempRWFaceInfo->RectY = top;
		tempRWFaceInfo->RectWidth = right - left;
		tempRWFaceInfo->RectHeight = bottom -top;
		//tempRWFaceInfo->RectType = type;
		if(type == 0)
		{
			tempRWFaceInfo->RectType = 4;
		}
		else
		{
			tempRWFaceInfo->RectType = 1;
		}
		
		tempRWFaceInfo->QScore = quality;

		//count people
		ret32 = FaceDetector::getInstance()->countPeople( tempRWFaceInfo );
		
		//FaceDetector::filterFace
		ret32 = FaceDetector::getInstance()->filterFace( tempRWFaceInfo);
		//refreshFaceNode
		if(ret32 == 0)
		{
			FaceDataManage::getInstance()->refreshFaceNode(tempRWFaceInfo);
		}
	}

	everyIntervalNotifyUse();// 5s send count people to user
	
	FaceDetector::getInstance()->selectFace( mRWFaceInfo);
	
	UINT8 detectType = DetectCfgInfo::getInstance()->getDetectType();


	//printf("(detectType != 0) && ((flags & 0x01) == 0x00) = %d\n",(detectType != 0) && ((flags & 0x01) == 0x00) );
	if( (detectType != 0) && ((flags & 0x01) == 0x00) )
	{
		//printf("--->>> mRWFaceInfo = %x\n",mRWFaceInfo);
		RWNotifyType pNotifyType = NotifyFaceInfo;
		UINT8 *pExtData = (UINT8 *)mRWFaceInfo;
		UINT32 pDataLen = dataLen;
		CallbackNotifyFunc  pfun = ProcessReq::getInstance()->getNotifyFaceInfoFun();
		(*pfun)( &pNotifyType, pExtData, &pDataLen);	
		delete [] mRWFaceInfo;
		//printf("ok");
		return 0;
	}
	delete [] mRWFaceInfo;
	//
	return -1;
}
NotifyFaceInfoUCmd::~NotifyFaceInfoUCmd()
{
}
