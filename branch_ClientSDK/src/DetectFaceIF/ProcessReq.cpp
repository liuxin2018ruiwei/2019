/********************************************************************
* Name      :  ProcessReq.cpp                                  
=* Describ   : the defination of the class
* Author    :  Zhang Botong                                          
* Date      :  2018-06-07                                                 
********************************************************************/

#include <string.h>
#include <stdio.h>
#include "DetectFaceIFCommon.h"
#include "DetectFaceIF.h"
#include "ProcessReq.h"
#include "CommunicationAgent.h"
#include "Requestion.h"
#include "UserCommonRequestion.h"
#include "ConfigManage.h"
#include "SendHeartBeatReq.h"
#include "HeartBeatInfo.h"
#include <time.h>
#include <sys/time.h>
#include "LittleEndianIO.h"
#include "DetectCfgInfo.h"
#include "SwitchDetectorReq.h"
#include "SwitchRequestion.h"
#include "BaseCaptureStratege.h"
#include "LeftAreaCaptureStratege.h"
#include "PthreadManage.h"
#include <FrameManage.h>
#include "MD5.h"
#include <string>
#include <iostream>
#include "CountInterface.h"
#include "ZmodeUpgrade.h"
#include <unistd.h>






ProcessReq *ProcessReq::m_this = nullptr;

ProcessReq* ProcessReq::getInstance()
{
	if( nullptr == m_this )
	{
		m_this = new ProcessReq;
	}

	return m_this;
}

int ProcessReq::getClientVersion( UINT8 *pVersion, int nLen )
{
	int nVerLen = ( nLen < VERSION_SIZE ) ? nLen : VERSION_SIZE;
	//printf("nVerLen = %d\n",nVerLen);

	memcpy( pVersion, SDK_VERSION_INFO, nVerLen );
	
	pVersion[nVerLen-1]= '\0';
	return 0;
}

int ProcessReq::sentStartUartFaceCheck()
{
	int flag = START_FACE_CHECK;
	Requestion *pReq = new SwitchRequestion( START_FACE_CHECK,
											 Requestion::SendSwitchDetectorReq );
	
	return CommunicationAgent::getInstance()->sendReq( pReq );
}

int ProcessReq::sentStopUartFaceCheck()
{
	Requestion *pReq = new SwitchRequestion( STOP_FACE_CHECK,
												 Requestion::SendSwitchDetectorReq );
	return CommunicationAgent::getInstance()->sendReq( pReq );
}


int  ProcessReq::setDetectConfig( DetectCfg *pCfg )
{
	int ret32 = 0;
	if(  NULL == pCfg )
	{
		printf("ERR %d %s  pConfig=%P\n", __LINE__, __func__, pCfg );
		return -1;
	}

	UINT32 tempMask = pCfg->ConfigMask;
	if(tempMask == 0)
	{
		return -1;
	}

	if((tempMask & MASK_DETECT_TYPE) == MASK_DETECT_TYPE)
	{
		ret32 |=  setDetectType(pCfg);
		DetectCfg tempCfg;
		tempCfg.DetectTime = DEFUTE_DETECT_TIME;
		ret32 |= setDetectTime(&tempCfg);
	}

	if((tempMask & MASK_DETECT_HEART_BEAT) == MASK_DETECT_HEART_BEAT)
	{
		ret32 |= setHeartBeat(pCfg);
	}

	if((tempMask & MASK_FACE_SIZE) == MASK_FACE_SIZE)
	{
		ret32 |= setFaceSize(pCfg);
	}
	if((tempMask & MASK_DETECT_TIME) == MASK_DETECT_TIME)
	{
		ret32 |= setDetectTime(pCfg);
	}

	if((tempMask & MASK_DETECT_THRESHOLD) == MASK_DETECT_THRESHOLD)
	{
		ret32 |= setDetectThreshold(pCfg);
	}

	
	if((tempMask & MASK_BACK_GROUND_CTRL) == MASK_BACK_GROUND_CTRL)
	{
		ret32 |= setBackGroundCtrl(pCfg);
	}

	if((tempMask & MASK_FACE_CTRL) == MASK_FACE_CTRL)
	{
		ret32 |= setFaceCtrl(pCfg);
	}
	//printf("set ret = %d\n",ret32);
	return ret32;
}


int ProcessReq:: setDetectType(DetectCfg *pCfg )
{
	if(pCfg == NULL)
	{
		printf("ERR: %d %s pCfg=%P\n", __LINE__, __func__, pCfg );
		return -1;
	}
	UINT8 newDetectType = pCfg->DetectType;

	UINT8 oldDetectType = DetectCfgInfo::getInstance()->getDetectType();

	// start check camera
	if( (oldDetectType == CAMERA_CLOSE) && (newDetectType !=  CAMERA_CLOSE))
	{
		sentStartUartFaceCheck();
	}
	//stop check  camera
	if( (oldDetectType != CAMERA_CLOSE) && (newDetectType == CAMERA_CLOSE) )
	{
		sentStopUartFaceCheck();
	}

	if( ( CAMERA_CLOSE > newDetectType )||( CAMERA_LEAVE_NOW_SANP < newDetectType))
	{
		printf("ERR: Out of Rang! %d %s  newDetectType = %d \n", __LINE__, __func__, newDetectType );
		return -1;
	}
	
	FaceDetector::getInstance()->setDetectType(newDetectType) ;
	return 0;
}


int ProcessReq:: setFaceSize(DetectCfg *pCfg )
{
	if(pCfg == NULL)
	{
		printf("ERR: %d %s pCfg=%P\n", __LINE__, __func__, pCfg );
		return -1;
	}
	UINT16 tempFaceSize = pCfg->FaceSize;
	if((MIN_FACE_SIZE > tempFaceSize) || (MAX_FACE_SIZE < tempFaceSize ))
	{
		printf("ERR: Out of Rang! %d %s pCfg=%P\n", __LINE__, __func__, pCfg );
		return -1;
	}
	
	DetectCfgInfo::getInstance()->setFaceSize(tempFaceSize);
	return 0;
}

int ProcessReq:: setDetectTime(DetectCfg *pCfg )
{
	if(pCfg == NULL)
	{
		printf("ERR: %d %s pCfg=%P\n", __LINE__, __func__, pCfg );
		return -1;
	}
	UINT8 tempDetectTime = pCfg->DetectTime;
	
	if( (MIN_DETECT_TIME_SIZE > tempDetectTime) || (MAX_DETECT_TIME_SIZE < tempDetectTime ) )
	{
		printf("ERR: Out of Rang! %d %s pCfg=%P\n", __LINE__, __func__, pCfg );
		return -1;
	}

	DetectCfgInfo::getInstance()->setDetectTime(tempDetectTime);
	return 0;
}

int ProcessReq:: setDetectThreshold(DetectCfg *pCfg )
{
	if(pCfg == NULL)
	{
		printf("ERR: %d %s pCfg=%P\n", __LINE__, __func__, pCfg );
		return -1;
	}
	UINT8 tempDetectThreshold = pCfg->DetectThreshold;

	if( (MIN_DETECT_THRESHOLD_SIZE > tempDetectThreshold) || ( MAX_DETECT_THRESHOLD_SIZE < tempDetectThreshold) )
	{
		printf("ERR: Out of Rang! %d %s pCfg=%P\n", __LINE__, __func__, pCfg );
		return -1;
	}
	
	DetectCfgInfo::getInstance()->setDetectThreshold(tempDetectThreshold);
	return 0;
}

int ProcessReq:: setBackGroundCtrl(DetectCfg *pCfg )
{
	if(pCfg == NULL)
	{
		printf("ERR: %d %s pCfg=%P\n", __LINE__, __func__, pCfg );
		return -1;
	}
	UINT8 tempBackGroundCtrl = pCfg->BackgroundCtrl;

	UINT8 tempH = tempBackGroundCtrl>>4;
	if((tempH == 0x00)||(tempH == 0x01))
	{
		;
	}
	else
	{
		printf("pCfg->BackgroundCtrl Hieght must 0x00 or 0x01 tempH = %x\n",tempH);
		return -1;
	}
	UINT8 tempLow = tempBackGroundCtrl & 0x0F;
	if((tempLow == 0x00) || (tempLow == 0x01) || (tempLow == 0x02) || (tempLow == 0x04) )
	{
		;
	}
	else
	{
		printf("pCfg->BackgroundCtrl LOW must 0x00 or 0x01 or 0x02 or 0x04 tempLow = %x\n",tempLow);
		return -1;
	}
	
	DetectCfgInfo::getInstance()->setBackGroundCtrl(tempBackGroundCtrl);
	return 0;
}

int ProcessReq:: setFaceCtrl(DetectCfg *pCfg )
{
	if(pCfg == NULL)
	{
		printf("ERR: %d %s pCfg=%x\n", __LINE__, __func__, pCfg );
		return -1;
	}
	UINT8 tempFaceCtrl = pCfg->FaceCtrl;

	UINT8 tempH = (tempFaceCtrl &0xF0)>>4;
	if((tempH == 0x01) || (tempH == 0x02) || (tempH == 0x04) || (tempH == 0x08))
	{
		//NOP;
	}
	else
	{
		printf("ERR:FaceCtrl Hight must 0x01 or 0x02 0r 0x04 or 0x08,FaceCtrl Hieght = %x \n",tempH);
		return -1;
	
}
	
	UINT8 tempL = tempFaceCtrl & 0x0F;
	if( (tempL == 0x01)||(tempL == 0x02) || (tempL == 0x04) )
	{
		//NOP;
	}else
	{
		printf("ERR FaceCtrl low must  0x01 or 0x02  0r 0x04 ,FaceCtrl = %d\n",tempL);
		return -1;
	}
	
	DetectCfgInfo::getInstance()->setFaceCtrl(tempFaceCtrl);
	return 0;
}


int ProcessReq:: setHeartBeat(DetectCfg *pCfg )
{
	if(pCfg == NULL)
	{
		printf("ERR: %d %s pCfg=%P\n", __LINE__, __func__, pCfg );
		return -1;
	}
	UINT8 heatBeat = pCfg->DetectHeartbeat;
	if( (MinHeatBeat > heatBeat) || ( MaxHeatBeat < heatBeat) )
	{
		printf("ERR: Out of Rang! %d %s pCfg=%P\n", __LINE__, __func__, pCfg );
		return -1; 
	}

	HeartBeatInfo::getInstance()->setHeatBeatInterval(heatBeat) ;
	return 0;
}

int ProcessReq::getDetectConfig( DetectCfg *pCfg )
{
	int ret32 = 0;
	if( NULL == pCfg )
	{
		printf("ERR %d %s TFHandle=%x, pConfig=%x\n", __LINE__, __func__, pCfg );
		return -1;
	}

	UINT32 tempMask = pCfg->ConfigMask;
	
	if(tempMask == 0)
	{
		printf("ERR : pCfg->ConfigMask= %d \n",pCfg->ConfigMask);
		return -1;
	}

	if((tempMask & MASK_DETECT_TYPE) == MASK_DETECT_TYPE)
	{
		ret32 |=  getDetectType(pCfg);
		//printf("1 ret32 = %d\n",ret32);

		ret32 |=  getDetectTime(pCfg);
		//printf("2 ret32 = %d\n",ret32);
	}

	if((tempMask & MASK_DETECT_HEART_BEAT) == MASK_DETECT_HEART_BEAT)
	{
		ret32 |=  getHeartBeat(pCfg);
		//printf("3 ret32 = %d\n",ret32);
	}

	if((tempMask & MASK_FACE_SIZE) == MASK_FACE_SIZE)
	{
		ret32 |=  getFaceSize(pCfg);
		//printf("4 ret32 = %d\n",ret32);
	}

	if((tempMask & MASK_DETECT_TIME) == MASK_DETECT_TIME)
	{
		ret32 |= getDetectTime(pCfg);
		//printf("5 ret32 = %d\n",ret32);
	}

	if((tempMask & MASK_DETECT_THRESHOLD) == MASK_DETECT_THRESHOLD)
	{
		ret32 |= getDetectThreshold(pCfg);
		//printf("6 ret32 = %d\n",ret32);
	}

	if((tempMask & MASK_BACK_GROUND_CTRL) == MASK_BACK_GROUND_CTRL)
	{
		ret32 |= getBackGroundCtrl(pCfg);
		//printf("7 ret32 = %d\n",ret32);
	}

	if((tempMask & MASK_FACE_CTRL) == MASK_FACE_CTRL)
	{
		ret32 |= getFaceCtrl(pCfg);
		//printf("8 ret32 = %d\n",ret32);
	}
	
	//printf("getDetectConfig ret32 = %d\n",ret32);
	return ret32;
		
}


// 向串口发送关闭检测，向串口发送软件升级；收到软件升级答复后，通知用户准备好升级；
// 1. close UART Check 
// 2. start  upgrade
 int ProcessReq::startUpgrade(UINT64 *pReqID, UpgradeDataInfo *pDataInfo,CallbackReplyFunc pFun)
 {
 
	 int ret = 0;
	 if( ( pDataInfo == NULL )||(NULL == pReqID) || (NULL == pFun) )
	 {
	 	printf("ERR: %d %s pDataInfo=%x,pReqID = %x,pFun = %x\n", __LINE__, __func__, pDataInfo,pReqID,pFun );
		return -1;	 	
	 }
	if((NULL == pDataInfo->FileNamPtr) || (NULL == pDataInfo->FilePathPtr) || (0 == pDataInfo->NameLen)||(0 == pDataInfo->PathLen)||(0 == pDataInfo->FileSize)) 
	{
		printf("ERR: pDataInfo->FileNamPtr =%x, pDataInfo->FilePathPtr = %x,pDataInfo->NameLen = %d,pDataInfo->PathLen = %d,pDataInfo->FileSize = %d\n",
			         pDataInfo->FileNamPtr,     pDataInfo->FilePathPtr,     pDataInfo->NameLen,     pDataInfo->PathLen,    pDataInfo->FileSize);
		
		return -1;
	}

	 int getFileLen = strlen((const char *)(pDataInfo->FilePathPtr));
	 if( (getFileLen != pDataInfo->PathLen - 1) || (pDataInfo->PathLen > MAX_UPGRAD_FILE_LEN) )
	 {
	 	printf("getFileLen = %d,pDataInfo->PathLen = %d\n",getFileLen,pDataInfo->PathLen);
		return -1;
	 }
	 int getNameLen = strlen((const char *) (pDataInfo->FileNamPtr));
	 if( (getNameLen != pDataInfo->NameLen-1) || (pDataInfo->NameLen > MAX_UPGRAD_NAME_LEN) )
	 {
		printf("getNamelen = %d, pDataInfo->PathLen = %d\n",getNameLen,pDataInfo->PathLen);
		return -1;
	 }
	 	
	 //check name and len 
	 UpgradeType tempUpgradeType = (UpgradeType)pDataInfo->UpgradeType;
	 if(APP_CFG_TYPE ==  tempUpgradeType)
	 {
		 if(0 != strncmp((const char*)(pDataInfo->FileNamPtr),APP_CFG_UPGRADE_NAME,pDataInfo->NameLen))
		 {
			 printf("APP_CFG_UPGRADE_NAME is not equle!\n");
			 return -1;
		 }
	 }
	 else if(APP_PKG_TYPE == tempUpgradeType)
	 {
		 if(0 != strncmp((const char*)(pDataInfo->FileNamPtr),APP_PKG_UPGRADE_NAME,pDataInfo->NameLen))
		 {
			 printf("APP_PKG_UPGRADE_NAME is not equle!");
			 return -1;
		 }
	 }
	 else if (CAMERA_QSPI_IMAGE_TYPE == tempUpgradeType)
	 {
		 if (0 != strncmp((const char*)(pDataInfo->FileNamPtr),CAMERA_QSPI_IMAGE_UPGRADE_NAME, pDataInfo->NameLen))
		 {
			 printf("CAMERA_QSPI_IMAGE_UPGRADE_NAME is not equle!");
			 return -1;
		 }
	 }
	else
	{
		printf("Err :get upgrade type = %d\n",tempUpgradeType);
		return -1;
	}

	 //保存升级数据，用于Zmode升级使用
	 //char fname[128];	
	 char *fname = FaceDetector::getInstance()->getUpdateFileName();
	 memcpy(fname, pDataInfo->FilePathPtr, pDataInfo->PathLen);
	 fname[pDataInfo->PathLen -1 ] = '/';
	 char *p = fname+ pDataInfo->PathLen;
	 memcpy( p, pDataInfo->FileNamPtr, pDataInfo->NameLen);
	 //printf("get fname = %s \n",fname);
	 UINT8 *pFileMD5 = pDataInfo->MD5;

	//test open file
	 FILE* file = fopen(fname, "rb");
	 if(file == NULL)
	 {
	 	printf("open ERR\n");
		return -1;
	 }
	 fclose(file);
	 file = NULL;

	 using std::string;
	 UINT8 pMyMD5[16] = {0};
	 string sMD5 = md5file((const char*)fname,(unsigned char *)pMyMD5);
	
	//test
/*	 for(int i = 0; i< 16*2; i++)
	 {
		printf("%c ", sMD5[i]);
	 }
	 printf("\n");
*/
	
	
/*	for(int i = 0; i < 16; i++)
	{
		printf("MD5 : pDataInfo->Md5[%d] = %x, pMyMD5[%d] = %x \n",i,pFileMD5[i],i,pMyMD5[i]);
	}
	*/
	for(unsigned int nCnt = 0; nCnt < 16; nCnt++ )
	{
		if( pMyMD5[nCnt] != pFileMD5[nCnt] )
		{
			//printf("pFileMD5::checkMD5 idx=%d pMyMD5=0x%X, pFileMD5=0x%X\n",
			//	                       nCnt, pMyMD5[nCnt], pFileMD5[nCnt] );
			return -1;
		}
	}
	
		
	 // 1. close UART Check
	 ret = sentStopUartFaceCheck();
	 //printf("[LX]  sentStopUartFaceCheck ret = %d\n",ret);
	 if(ret != 0)
	 {
	 	printf("ERR: %d %s ret=%d\n", __LINE__, __func__, ret );
		return -1;
	 }

	 //sleep(1);
	 // 2. start  upgrade
	 m_pCallbackUpgradeFunc = pFun; //save CallbackReplyFunc
	 Requestion *rq = new UpgradeRequestion(Requestion::SoftUpgrade, *pDataInfo);
	 ret =  CommunicationAgent::getInstance()->sendReq( rq );
	 if(ret != 0)
	 {
		printf("ERR: %d %s ret=%d\n", __LINE__, __func__, ret );
		return -1;
	 }
	 
	 return ret;
 }


int  ProcessReq::notifToRestart()
{

	//printf("notif To....\n");

	//调用回调函数通知用户升级结束，应该重启
	CallbackReplyFunc pReplyFunc = m_pCallbackUpgradeFunc;
	RWNotifyType myRWNotifyType = NotifyState;
	UINT8 pRet = 0;
	UINT32 pDataLen = 1;
	UINT64 reqID = Requestion::SoftUpgrade;

	UINT8 pReplyData = UpgradeReqType :: ReqStopUpgrade;
	pRet = 0;
	pDataLen = 1;
	(*pReplyFunc)(&reqID,  &pRet, &pReplyData, &pDataLen);

	//printf("restart...\n");
	return 0;
}


int ProcessReq::getDetectType(DetectCfg *pCfg)
{
	if(pCfg == NULL)
	{
		printf("ERR: %d %s pCfg=%x\n", __LINE__, __func__, pCfg );
		return -1;
	}
	UINT8  tempDetectType = DetectCfgInfo::getInstance()->getDetectType();
	if( ( CAMERA_CLOSE > tempDetectType ) || ( CAMERA_LEAVE_NOW_SANP < tempDetectType ))
	{
		printf("ERR: Out of Rang! %d %s pCfg=%P\n", __LINE__, __func__, pCfg );
		return -1;
	}
	 
	pCfg->DetectType = tempDetectType;
	return 0;
}


int ProcessReq:: getHeartBeat(DetectCfg *pCfg )
{
	if(pCfg == NULL)
	{
		printf("ERR: %d %s pCfg=%x\n", __LINE__, __func__, pCfg );
		return -1;
	}

	UINT8 heatBeat = HeartBeatInfo::getInstance()->getHeatBeatInterval();
	if( (MinHeatBeat > heatBeat) || ( MaxHeatBeat < heatBeat) )
	{
		printf("ERR: Out of Rang! %d %s pCfg=%P\n", __LINE__, __func__, pCfg );
		return -1;
	}
	pCfg->DetectHeartbeat = heatBeat;
	return 0;
}

int  ProcessReq::getFaceSize( DetectCfg *pCfg )
{
	if(pCfg == NULL)
	{
		printf("ERR: %d %s pCfg=%x\n", __LINE__, __func__, pCfg );
		return -1;
	}

	UINT16 tempFaceSize = DetectCfgInfo::getInstance()->getFaceSize();
	if((MIN_FACE_SIZE > tempFaceSize) || (MAX_FACE_SIZE < tempFaceSize ))
	{
		printf("ERR: Out of Rang! %d %s pCfg=%P\n", __LINE__, __func__, pCfg );
		return -1;
	}
	pCfg->FaceSize= tempFaceSize;
	return 0; 
}

int  ProcessReq::getDetectTime( DetectCfg *pCfg )
{
	if(pCfg == NULL)
	{
		printf("ERR: %d %s pCfg=%x\n", __LINE__, __func__, pCfg );
		return -1;
	}
	UINT8 tempDetectTime = DetectCfgInfo::getInstance()->getDetectTime();
	if( (MIN_DETECT_TIME_SIZE > tempDetectTime) || (MAX_DETECT_TIME_SIZE < tempDetectTime ) )
	{
		printf("ERR: Out of Rang! %d %s pCfg=%P\n", __LINE__, __func__, pCfg );
		return -1;
	}
	pCfg->DetectTime= tempDetectTime;
	return 0;
}

int  ProcessReq::getDetectThreshold( DetectCfg *pCfg )
{
	if(pCfg == NULL)
	{
		printf("ERR: %d %s pCfg=%x\n", __LINE__, __func__, pCfg );
		return -1;
	}
	UINT8 tempDetectThreshold = DetectCfgInfo::getInstance()->getDetectThreshold();

	if( (MIN_DETECT_THRESHOLD_SIZE > tempDetectThreshold) || ( MAX_DETECT_THRESHOLD_SIZE < tempDetectThreshold) )
	{
		printf("ERR: Out of Rang! %d %s pCfg=%P\n", __LINE__, __func__, pCfg );
		return -1;
	}
	 
	pCfg->DetectThreshold = tempDetectThreshold;
	return 0;
}


int  ProcessReq::getBackGroundCtrl( DetectCfg *pCfg )
{
	if(pCfg == NULL)
	{
		printf("ERR: %d %s pCfg=%x\n", __LINE__, __func__, pCfg );
		return -1;
	}
	UINT8 tempBackGroundCtrl = DetectCfgInfo::getInstance()->getBackGroundCtrl();

	if( (MIN_BACK_GROUND_CTRL > tempBackGroundCtrl) || ( MAX_BACK_GROUND_CTRL < tempBackGroundCtrl) )
	{
		printf("ERR: Out of Rang! %d %s pCfg=%P\n", __LINE__, __func__, pCfg );
		return -1; 
	}
	
	pCfg->BackgroundCtrl= tempBackGroundCtrl;
	return 0 ;
}

int  ProcessReq::getFaceCtrl( DetectCfg *pCfg )
{
	if(pCfg == NULL)
	{
		printf("ERR: %d %s pCfg=%x\n", __LINE__, __func__, pCfg );
		return -1;
	}
	UINT8 tempFaceCtrl = DetectCfgInfo::getInstance()->getFaceCtrl();

	if( (MIN_FACE_CTRL > tempFaceCtrl) || ( MAX_FACE_CTRL < tempFaceCtrl) )
	{
		printf("ERR: Out of Rang! %d %s pCfg=%P\n", __LINE__, __func__, pCfg );
		return -1;
	}
	pCfg->FaceCtrl= tempFaceCtrl;
	return 0;
}

int  ProcessReq::PointInImgRect( Point *pPoint, int nCount )
{
	//判断点是否在图片中
	InitImgInfo* getImagSize =  FrameManage::getInstance()->getFrameInfo();
	int width = getImagSize->Width;
	int height = getImagSize->Height;
	Point *pTemp = NULL;
	//printf("width = %d, height = %d\n",width,height);
	for(int i = 0 ; i < nCount; i++)
	{
		pTemp = pPoint + i;
		
		if ( (pTemp->X > width) || (pTemp->Y > height))
		{
			printf("pPoint is Err! pPoint[%d].X = %d ; pPoint[%d].Y = %d, width = %d,height = %d\n",
				     				i, pTemp->X, i,pTemp->Y,width,height);
			return -1;
		}
	}
	return 0;
}


//This function is used to increase the face detection range
int ProcessReq::addDetectRegion( Point *pPoint, int nCount )
{
	if( ( NULL == pPoint ) || (nCount < MIN_POINT_NUM) || (nCount > MAX_POINT_NUM))
	{
		printf("ERR %d %s , pConfig=%P nCount =  %d\n", __LINE__, __func__, pPoint, nCount);
		return -1;
	}

	if(FaceDetector::getInstance()->getListShieldRegion().size() > 0)
	{
		printf(" ERR  %d %s,addDetectRegion : getListShieldRegion= %d\n", __LINE__, __func__,FaceDetector::getInstance()->getListShieldRegion().size());
		return -1;
	}

	
	int ret32 = PointInImgRect(pPoint, nCount);
	if(ret32 != 0)
	{
		printf("ERR %d %s , point is not legal.\n", __LINE__, __func__);
		return -1;
	}

	return FaceDetector::getInstance()->addDetectRegion(pPoint,nCount);
	
}

//This function is used to delete the face detection range
int ProcessReq::deletetDetectRegion( Point *pPoint, int nCount )
{
	if( ( NULL == pPoint ) || (nCount < MIN_POINT_NUM ) || (nCount > MAX_POINT_NUM))
	{
		printf("ERR %d %s , pConfig=%x nCount =  %d\n", __LINE__, __func__, pPoint, nCount);
		return -1;
	}

	int ret32 = PointInImgRect(pPoint, nCount);
	if(ret32 != 0)
	{
		printf("ERR %d %s , point is not legal.\n", __LINE__, __func__);
		return -1;
	}
	
	return FaceDetector::getInstance()->deletetDetectRegion(pPoint,nCount);
}

int ProcessReq::addShieldRegion( Point *pPoint, int nCount )
{
	if( ( NULL == pPoint ) || (nCount < MIN_POINT_NUM ) || (nCount > MAX_POINT_NUM))
	{
		printf("ERR %d %s , pConfig=%x nCount =  %d\n", __LINE__, __func__, pPoint, nCount);
		return -1;
	}
	if(FaceDetector::getInstance()->getlistRegion().size() > 0)
	{
		printf("ERR %d %s ,  addShieldRegion;getlistRegion().size() = %d\n",__LINE__, __func__, FaceDetector::getInstance()->getlistRegion().size());
		return -1;
	}

	int ret32 = PointInImgRect(pPoint, nCount);
	if(ret32 != 0)
	{
		printf("ERR %d %s , point is not legal.\n", __LINE__, __func__);
		return -1;
	}
	

	return FaceDetector::getInstance()->addShieldRegion(pPoint,nCount);
	
}

int ProcessReq::deletetShieldRegion (  Point *pPoint, int nCount )
{
	if( ( NULL == pPoint ) || (nCount < MIN_POINT_NUM) || (nCount > MAX_POINT_NUM))
	{
		printf("ERR %d %s , pConfig=%P nCount =  %d\n", __LINE__, __func__, pPoint, nCount);
		return -1;
	}

	return FaceDetector::getInstance()->deletetShieldRegion(pPoint,nCount);
	
}


int ProcessReq::getFaceDetectVersionInfo( VersionInfo *pVersion)
{
	if(nullptr == pVersion)
	{	
		printf("ERR %d %s , pVersion =  %d\n", __LINE__, __func__, pVersion);
		return -1;
	}
	ConfigManage::getInstance()->getModuleProtocolVersion( pVersion->ProtocolVersion,
														   VERSION_SIZE );
	ConfigManage::getInstance()->getModuleSoftwareVersion( pVersion->SoftwareVersion,
														   VERSION_SIZE );
	ConfigManage::getInstance()->getModuleFirmwareVersion( pVersion->FirmwareVersion,
														   VERSION_SIZE );

	return 0;
}


int ProcessReq::getServiceSDKVersion( char * protocolVersion ,int len)
{
	if( (nullptr == protocolVersion) || (len <= 0) )
	{
		return -1;
	}

	//return ConfigManage::getInstance()->getModuleProtocolVersion( protocolVersion, len );
	return 0;
}

int ProcessReq::initProcessReq( void * priData )
{
#ifdef DEBUG_LOG
	printf("%d : %s\n", __LINE__, __func__);
#endif	
	
	if( nullptr == priData )
	{
		printf("ERR: %d %s pConfig=%P\n", __LINE__, __func__, priData );
		
		return -1;
	}

	// init parameter
	memset( &m_callback, 0, sizeof( CallBackInfo ) );
	m_callback.RegisterReplyFunc = callbackRelpyRegisterClient;
	m_callback.CancelRelpyFunc = callbackReplyCancalClient;
	m_callback.NotifyFaceFunc = callbackNotifyFaceInfo;
	m_callback.NotifyClientFunc = callbackNotifyServiceInfo;
	m_pFuncCountPeople = (m_callback.NotifyFaceFunc);
	resetCounter();
	m_pFaceEncodeCallbackFunc = NULL;
	m_pBackgroundEncodeCallFunc = NULL;
	return 0;
}

void ProcessReq::uninitProcessReq()
{
	resetCounter();
}


int ProcessReq::registerCallback( CallBackInfo * pCallbackFunc )
{
#ifdef DEBUG_LOG
	printf("%d : %s\n", __LINE__, __func__);
#endif
	if( nullptr == pCallbackFunc )
	{
		printf("ERR %d %s pCallback=%p\n", __LINE__, __func__,
			   pCallbackFunc );
		return -1;
	}

	if( pCallbackFunc->RegisterReplyFunc != nullptr )
	{
		m_callback.RegisterReplyFunc = pCallbackFunc->RegisterReplyFunc;
	}
	else
	{
		printf("ERR: pCallbackFunc->RegisterReplyFunc = %x\n",pCallbackFunc->RegisterReplyFunc);
		return -1;
	}
	
	if( pCallbackFunc->CancelRelpyFunc != nullptr )
	{
		m_callback.CancelRelpyFunc = pCallbackFunc->CancelRelpyFunc;
	}
	else
	{
		printf("ERR: pCallbackFunc->CancelRelpyFunc = %x\n",pCallbackFunc->CancelRelpyFunc);
		return -1;
	}
	
	if( pCallbackFunc->NotifyFaceFunc != nullptr )
	{
		m_callback.NotifyFaceFunc = pCallbackFunc->NotifyFaceFunc;
	}
	else
	{
		printf("ERR: pCallbackFunc->NotifyFaceFunc = %x\n",pCallbackFunc->NotifyFaceFunc);
		return -1;
	}
	
	if( pCallbackFunc->NotifyClientFunc != nullptr )
	{
		m_callback.NotifyClientFunc = pCallbackFunc->NotifyClientFunc;
	}
	else
	{
		printf("ERR: pCallbackFunc->NotifyClientFunc = %x\n",pCallbackFunc->NotifyClientFunc);
		return -1;
	}
	
	return 0;
}

int ProcessReq:: registerEncodeCallback(EncodeCallBackInfo *pEncodeCallbackInfo)
{
	if(NULL == pEncodeCallbackInfo)
	{
		return -1;
	}
	if(NULL != pEncodeCallbackInfo->RegisterEncodeFunc)
	{
		m_pFaceEncodeCallbackFunc = pEncodeCallbackInfo->RegisterEncodeFunc;
	}
	if(NULL != pEncodeCallbackInfo->RegisterBackgroundEncodeFunc)
	{
		m_pBackgroundEncodeCallFunc = pEncodeCallbackInfo->RegisterBackgroundEncodeFunc;
	}
	return 0;
}

CallbackFaceEncodeFunc ProcessReq:: getFaceEncodeCallbackFunc()
{
	return m_pFaceEncodeCallbackFunc;
}

CallbackBackGroundEncodeFunc ProcessReq:: getBackGroundEncodeFunc()
{
	return m_pBackgroundEncodeCallFunc;
}


int ProcessReq::startPthread()
{
	int nRet = PthreadManage::getInstance()->startPthread();
	if( nRet != 0 )
	{
		return nRet;
	}
	//不能放上面初始化，不然卸载重启会有错
	Communication::getInstances()->start();
	return nRet;
}
CallbackNotifyFunc ProcessReq::getClientNotifyFun()
{
	return m_callback.NotifyClientFunc;
}

CallbackReplyFunc   ProcessReq:: getUpgradeReplyFunC()

{
	return m_pCallbackUpgradeFunc;
}

CallbackNotifyFunc ProcessReq::getNotifyFaceInfoFun()
{
	return m_callback.NotifyFaceFunc;
}

int ProcessReq::addFrame( UINT8 *pImage )
{
	if(NULL == pImage)
	{
		printf("ERR %d %s pImage=%x\n", __LINE__, __func__,
			   pImage );
		return -1;
	}
	return FrameManage::getInstance()->addFrame( pImage );
}


CallbackNotifyFunc  ProcessReq::getFuncCountPeopl()
{

	if(NULL == m_pFuncCountPeople)
	{
		printf("ERR :m_pFuncCountPeople =%x\n ",m_pFuncCountPeople);
		return NULL;
	}
	//printf("getFuncCountPeopl m_pFuncCountPeople = %x\n",m_pFuncCountPeople);
	return m_pFuncCountPeople;
}



//在ProcessReq中添加参考线时，对参考线有效性判断，
//判断参考线当x=0时，x=width时，y是否都大于0；注意参考系x向右为正，y向下为正
int ProcessReq::isvalideLine( Line *line )
{
	if(NULL == line)
	{
		return -1;
	}
	
	Line *myLine = line;
	int getWidth = FrameManage::getInstance()->getFrameInfo()->Width;
	int getHeight = FrameManage::getInstance()->getFrameInfo()->Height;
	

	Point maxP,minP;
	if(myLine->Head.X < myLine->End.X)
	{
		minP.X = myLine->Head.X;
		minP.Y = myLine->Head.Y;
		maxP.X = myLine->End.X;
		maxP.Y = myLine->End.Y;
	}
	else
	{
		minP.X = myLine->End.X;
		minP.Y = myLine->End.Y;
		maxP.X = myLine->Head.X;
		maxP.Y = myLine->Head.Y;	
	}

	if( ( 0 == minP.X ) && ( 0 == minP.Y ) && (getWidth== maxP.X) && (0 == maxP.Y) )
	{
		return -1;
	}
	
	else if( ( 0 == minP.X) && ( getHeight== minP.Y) && (getWidth == maxP.X) && (getHeight == maxP.Y) )
	{
		return -1;
	}
/*
	if( (minP.X != 0) || (maxP.X != getWidth) )
	{
		return -1;
	}
*/

	// y = k*x + b;
	float k = (float)(maxP.Y - minP.Y)/(maxP.X - minP.X);
	int b = maxP.Y - (int)k * maxP.X;

	if (minP.X != 0)
	{
		minP.X = 0;
		minP.Y = b;
	}

	if(maxP.X != getWidth)
	{
		maxP.X = getWidth;
		maxP.Y = (int)(k * getWidth) + b;
	}

	line->Head.X = minP.X;
	line->Head.Y = minP.Y;
	line->End.X = maxP.X;
	line->End.Y = maxP.Y;

	//printf("LINE is ok ! line->Head.X = %d,line->Head.Y = %d,line->End.X = %d,line->End.Y = %d\n",
	//								line->Head.X, line->Head.Y,line->End.X,line->End.Y );
	
	return 0;
	
}


int ProcessReq::setLine( Line *pLine, CallbackNotifyFunc pFunc )
{
	if( NULL == pLine ) 
	{
		printf("setLine pLine = %x\n",pLine);
		return -1;
	}

	int listSize = CountInterface::getInstance()->getCounterList().size();
	if( listSize >= MAX_LINE_NUM )
	{
		printf("ERR: Line num = %d is out of range. \n", listSize );
		return -1;
	}

	if(0 != isvalideLine(pLine))
	{
		printf("isvalideLine is not pass !\n");
		return -1;
	}
	
	m_pFuncCountPeople = pFunc;
	CountInterface::Line myLine;
	memcpy( &myLine, pLine, sizeof(CountInterface::Line) );
	CountInterface::getInstance()->setStandardLine( myLine );
	//printf("set line ok!setStandardLine->listSize = %d\n",CountInterface::getInstance()->getCounterList().size());
	return 0;
}





int ProcessReq::resetCounter()
{
	int ret = 0;
	ret = CountInterface::getInstance()->resetCounterInfo();
	ret |= CountInterface::getInstance()->cleanupLine();
	//printf("resetCounter : size = %d \n",CountInterface::getInstance()->getCounterList().size());

	return ret;
}


ProcessReq::~ProcessReq()
{
}

ProcessReq::ProcessReq()
{
}
