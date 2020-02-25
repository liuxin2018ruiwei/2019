/********************************************************************
* Name      :  EncodeModule.cpp                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                     
* Author    :  liuxin                                        
* Date      :  2018-06-25                                                 
********************************************************************/
#include <stdio.h>
#include "EncodeModule.h"
#include "FaceDataManage.h"
#include "FaceDetector.h"
#include "ProcessReq.h"
#include <cstring>
#include <FrameManage.h>

#include "MemoryNode.h"
#include "MemoryManager.h"
#include "PthreadManage.h"
#include "Version.h"


#ifndef X86COMPILE
#include "HaisiEcode.h"
#endif


//extern "C" {
//#include "turbojpeg.h"
//}



EncodeModule* EncodeModule::m_this =  nullptr;

EncodeModule* EncodeModule::getInstance()
{
	if( nullptr == m_this )
	{
		m_this = new EncodeModule;
	}

	return m_this;
}



int EncodeModule::processFaceInfo(FaceNode * pFaceNode)
{
	int ret32 = 0;
	//printf("processFaceInfo -> pFaceNode \n\n\n");
	//EncodeModule::processFaceInfo()中，检查Facedetctor::FaceCtrl是否打开人脸图
	///人脸图控制;（高4位控制人脸图类型，低4位控制人脸图压缩率）0x10：人脸图;0x20 头肩照;0x40 半身照; 0x80 全身照;0x01压缩率25&的人脸图，0x02压缩率60%的人脸图;0x04压缩率90%的人脸图
	UINT8 compressLevel= DetectCfgInfo::getInstance()->getFaceCompressLevel();
	if( ERR_FACE_COMPRESS_LEVEL == compressLevel )
	{
		return -1;
	}

	Rect destRect;
	Rect srcRect;
	srcRect.left= pFaceNode->RectX;
	srcRect.right= pFaceNode->RectX + pFaceNode->RectWidth;
	srcRect.top = pFaceNode->RectY;
	srcRect.bottom = pFaceNode->RectY + pFaceNode->RectHeight;
	//printf("srcRect.left = %u,srcRect.right  = %u,srcRect.top = %u,srcRect.bottom =%u \n",srcRect.left,srcRect.right,srcRect.top,srcRect.bottom);
	

	//人脸图控制; 0x10：人脸图;0x20 头肩照;0x40 半身照; 0x80 全身照;
	FaceType targerType = (FaceType)(DetectCfgInfo::getInstance()->getFaceType());//高位处理向后移动4个字节
	//printf("targerType = %d\n",targerType);
	FaceRectType newType = (FaceRectType)(pFaceNode->RectType);//人脸框类型： 1：头肩框， 2：半身照框，3：全身照框， 4：人脸框
	//printf("newType = %d\n",newType);
	FaceRectType newTagerType = (FaceRectType)changeFaceTypeToFaceRectType(targerType);
	//printf("pFaceNode->TrackNo =%llx,newType = 0x%x,newTagerType = 0x%x \n",pFaceNode->TrackNo,newType,newTagerType);
	ret32 = scaleRect( &destRect, &srcRect, newType, newTagerType);//框的缩放规则
	//printf("destRect.left = %u,destRect.right  = %u,destRect.top = %u,destRect.bottom =%u \n",destRect.left,destRect.right,destRect.top,destRect.bottom);
	if(0 != ret32)
	{
		printf("face is out of frame!\n");
		return -1;
	}


#ifndef HAISI_ENCODE_BY_USER
	// background
	UINT8 *pBackGroundPic =  FrameManage::getInstance()->getImage(pFaceNode->frameNumber);
	if(NULL == pBackGroundPic)
	{
		printf("ERR : getback pictrue is NULL! pBackGroundPic = %d\n",pBackGroundPic);
		return -1;
	}
#endif

	//printf("pBackGroundPic = %x,pFaceNode->TrackNo = %llx\n",pBackGroundPic,pFaceNode->TrackNo);
	int backGoundPicWidth =  FrameManage::getInstance()->getFrameInfo()->Width;
	int backGoundPicHeight = FrameManage::getInstance()->getFrameInfo()->Height;
	int nLen = backGoundPicWidth * backGoundPicHeight * 1.5;


	//RECT
	int rectWidth = destRect.right - destRect.left;
	int rectHight = destRect.bottom  - destRect.top;
	rectWidth = (rectWidth>>2)<<2;
	destRect.right = destRect.left + rectWidth;
	rectHight = (rectHight>>2)<<2;
	destRect.bottom = destRect.top + rectHight;

	//printf("****[rectWidth = %d][rectHight = %d]*****",rectWidth,rectHight);
	const int faceRectLen = rectWidth * rectHight * 3;//BGR 分量
	MemoryNode facenode(faceRectLen);
	unsigned char * faceRectPic = (unsigned char*)facenode.getMemory();
	if( nullptr == faceRectPic )
	{
		//printf("faceRectPic is NULL\n");
		return -1;		
	}
	

	//notify face image to user
	//0x10压缩率25&的人脸图，0x20压缩率60%的人脸图;0x40压缩率90%的人脸图
	UINT8 faceCompressLevel = DetectCfgInfo::getInstance()->getFaceCompressLevel();
	if(faceCompressLevel == 2 ) 
	{
		faceCompressLevel = 60;
	}else if(faceCompressLevel == 1 ) 
	{
		faceCompressLevel = 25;
	}else if(faceCompressLevel == 4 ) 
	{
		faceCompressLevel = 90;
	}
	//printf("faceCompressLevel = %d\n ",faceCompressLevel);
	//获取用户自定义编码函数
	CallbackFaceEncodeFunc userEncodeFun = ProcessReq::getInstance()->getFaceEncodeCallbackFunc();
	if( ERR_FACE_COMPRESS_LEVEL !=  faceCompressLevel )
	{
		//截图，截小图
		if(NULL == userEncodeFun)
		{
#ifndef HAISI_ENCODE_BY_USER
			int outlen = 0;
			ret32 = encodeImg( pBackGroundPic, (UINT8 *)faceRectPic,destRect.left,destRect.top, (rectWidth>>4)<<4, (rectHight>>1)<<1,  &outlen, faceCompressLevel, (int)Jpg);//转 jpg 
			ret32 = NotifyImgDataToUser(faceRectPic, outlen,pFaceNode);
#endif

		}
		else
		{
			RWFaceInfo rectInfo;
			rectInfo.ImgID = pFaceNode->frameNumber;
			rectInfo.QScore = pFaceNode->QScore;
			rectInfo.RectHeight = (rectWidth>>4)<<4;//用转换后的坐标和宽高
			rectInfo.RectWidth = (rectHight>>1)<<1;
			rectInfo.RectX = destRect.left;
			rectInfo.RectY = destRect.top;
			rectInfo.TrackNo = pFaceNode->TrackNo;
			rectInfo.RectType = pFaceNode->RectType;
			//printf("pFaceNode->frameNumber = %lld, pFaceNode->TrackNo = %lld\n",pFaceNode->frameNumber, pFaceNode->TrackNo);
			//printf("pFaceNode->RectX = %d,pFaceNode->RectY = %d,rectInfo.RectHeight = %d,rectInfo.RectWidth = %d \n",
			//	rectInfo.RectX,rectInfo.RectY,rectInfo.RectHeight,rectInfo.RectWidth);
			ret32 = (*userEncodeFun)(pFaceNode->frameNumber, rectInfo, faceCompressLevel, NULL);
		}
	}

	//notify back image to user
	//背景图上传控制;高4位控制人脸框叠加，低4位控制背景压缩率，
	//0x00：不上传背景图，0x01：压缩率25%的背景图;0x02：压缩率60%的背景图，0x04：压缩率90%的背景图;//低4位
	//0x00（高4位0）：背景图不叠加人脸;0x10：背景图叠加人脸框
	UINT8 backCompressLevel = DetectCfgInfo::getInstance()->getBackGrundCompressLevel(); //低4位
	if(1 == backCompressLevel )
	{
		backCompressLevel = 25;
	}
	else if(2 == backCompressLevel)
	{
		backCompressLevel = 60;
	}
	else if (4 == backCompressLevel )
	{
		backCompressLevel = 90;
	}
	//printf("backCompressLevel =%d \n",backCompressLevel);
	if( backCompressLevel != 0)//发送叠加还是不叠加的背景图
	{
		UINT8 backGrundSuperstack = DetectCfgInfo::getInstance()->getBackGrundSuperstack();// 高4位
		const int outLen = backGoundPicWidth*backGoundPicHeight*3;
		UINT8 out[outLen];

		//printf("backGrundSuperstack :%d\n",backGrundSuperstack);
		if(1 == backGrundSuperstack)//叠加背景框，
		{
			FD_RGN_CROP_UNIT tempCrop;
			tempCrop.crop_start_x = srcRect.left;
			tempCrop.crop_start_y = srcRect.top;
			tempCrop.crop_width = srcRect.right - srcRect.left;
			tempCrop.crop_height = srcRect.bottom - srcRect.top;
			tempCrop.type = 0;
#ifndef HAISI_ENCODE_BY_USER
			drawer.setImageSize(backGoundPicWidth,backGoundPicHeight);
			drawer.drawRect(( unsigned char *)pBackGroundPic, tempCrop);
#endif
		}

		//printf("encode background pic---\n\n");
		CallbackBackGroundEncodeFunc userBackgroundFunc = ProcessReq::getInstance()->getBackGroundEncodeFunc();
		if(NULL == userBackgroundFunc)
		{
#ifndef HAISI_ENCODE_BY_USER
			//背景图发送
			int outlen = 0;
			ret32 = encodeImg( pBackGroundPic, (UINT8 *)&out, 0,0, backGoundPicWidth, backGoundPicHeight,  &outlen, backCompressLevel, (int)Jpg);// 转 jpg
			ret32 = NotifyImgDataToUser(out, outlen,NULL);
#endif
		}
		else
		{
			ret32 = (*userBackgroundFunc)(pFaceNode->frameNumber,backCompressLevel,NULL);
		}
	}
	
	return ret32;
	
}

int EncodeModule::NotifyImgDataToUser(UINT8 *Image, int len,FaceNode * pFaceNode)
{
	if((Image == NULL) || (0 == len))
	{
		printf("ERR Image = %x,len = %d\n",Image,len);
		return -1;
	}
	RWNotifyType pNotifyType = NotifyImgData;
	//RWFaceInfo;
	//RWImgInfo;
	FaceAndImgInfoData imgData;
	if(NULL != pFaceNode)//用pFaceNode参数 有2个用处，第一传参，第二判断是那个调用这个函数
	{
		imgData.imgInfoData.ImgPtr = (UINT32 *)Image;
		imgData.imgInfoData.Height = pFaceNode->RectHeight;
		imgData.imgInfoData.Width = pFaceNode->RectWidth;
		imgData.imgInfoData.ImgLen = len;//face pic
	//	printf("face pic is face pic! len = %d,imgData.imgInfoData.ImgPtr = %x\n", 
	//		 	imgData.imgInfoData.ImgLen, imgData.imgInfoData.ImgPtr);
	}
	else
	{
		//背景图上传
		imgData.imgInfoData.ImgPtr = (UINT32 *)Image;
		imgData.imgInfoData.Height = FrameManage::getInstance()->getFrameInfo()->Height; 
		imgData.imgInfoData.Width = FrameManage::getInstance()->getFrameInfo()->Width;
		imgData.imgInfoData.ImgLen = len;//JPG  background Pic
	//	printf("background pic is face background! len = %d,imgData.imgInfoData.ImgPtr = %x\n", 
	//		 	imgData.imgInfoData.ImgLen, imgData.imgInfoData.ImgPtr);
	}
	
	//imgData.faceInfoData.
	//printf("send NotifyImgDataToUser \n\n");
	
	UINT8 *pExtData = (UINT8 *) &imgData;// Image;
	UINT32 pDataLen =  sizeof(imgData);//len;
	CallbackNotifyFunc  pfun = ProcessReq::getInstance()->getNotifyFaceInfoFun();
	(*pfun)( &pNotifyType, (UINT8 *)&imgData, &pDataLen);
}


int EncodeModule::changeFaceTypeToFaceRectType(FaceType & faceType)
{
	//face node 人脸图控制; 0x10：人脸图; 0x20 头肩照; 0x40 半身照;  0x80 全身照;
	//face rect:人脸框类型： 1：头肩框， 2：半身照框，3：全身照框， 4：人脸框
	if(FACE_MAP == faceType)
	{
		return (int)FACE_RECT;
	}
	else if(HEAD_SHOULDER_PHOTOGRAPH == faceType)
	{
		return (int)FACE_SHOULD_RECT;
	}
	else if(HALF_BODY == faceType)
	{
		return (int)HALF_BODY_RECT;
	}
	else if(FULL_BODY_PICTRUE == faceType)
	{
		return (int)FULL_BODY_RECT;
	}
	return -1;
}
int EncodeModule::scaleRect(Rect * destRect, Rect * srcRect, FaceRectType newFaceType, FaceRectType targerType)
{
	enum
	{
		lr_aval = 0,
		left_off = 1,
		top_off = 2,
		right_off = 3,
		bott_off = 4,
	};
	float faceToFace[5]      = 	{0.747013, 0.747825, 0.632334, 0.7462,   0.412873};
	float shouldToFace[5]    = 	{0.886037, 0.848101, 0.409214, 0.923972, 0.148645};
	float faceToShould[5]    = 	{1.295283, 1.312994, 0.8925,   1.277572, 1.787918};
	float shouldToShould[5]  =	{1.302343, 1.092215, 0.435256, 1.51247,  1.182664};
	float faceToHalfBody[5]  =	{1.344703, 1.317768, 0.869773, 1.371638, 3.356874};
	float shouldToHalfBody[5]=  {1.426687, 1.212987, 0.435256, 1.640387, 1.988749};
	float faceToBody[5]      =  {1.420495, 1.294071, 0.8517,   1.54692,  8.087961};
	float shouldToBody[5]    =  {1.638341, 1.146676, 0.435256, 2.130006, 5.130232};

	float *temp = faceToFace;
	
	//faceToFace
	if(((int)FACE_RECT == (int)newFaceType)&&((int)FACE_RECT == (int)targerType))
	{
		temp = faceToFace;
		//printf("faceToFace\n");
	}
	else if(((int)FACE_SHOULD_RECT == (int)newFaceType)&&((int)FACE_RECT == (int)targerType))
	{
		temp = shouldToFace;
		//printf("shouldToFace\n");
	}
	else if(((int)FACE_RECT == (int)newFaceType)&&((int)FACE_SHOULD_RECT == (int)targerType))
	{
		temp = faceToShould;
		//printf("faceToShould\n");
	}
	else if(((int)FACE_SHOULD_RECT == (int)newFaceType)&&((int)FACE_SHOULD_RECT == (int)targerType))
	{
		temp = shouldToShould;
		//printf("shouldToShould\n");
	}
	else if(((int)FACE_RECT  == (int)newFaceType)&&((int)HALF_BODY_RECT == (int)targerType))
	{
		temp = faceToHalfBody;
		//printf("faceToHalfBody\n");
	}
	else if(((int)FACE_SHOULD_RECT == (int)newFaceType)&&((int)HALF_BODY_RECT == (int)targerType))
	{
		temp = shouldToHalfBody;
		//printf("shouldToHalfBody\n");
	}
	else if(((int)FACE_RECT == (int)newFaceType)&&((int)FULL_BODY_RECT == (int)targerType))
	{
		temp = faceToBody;
		//printf("faceToBody\n");
	}
	else if(((int)FACE_SHOULD_RECT == (int)newFaceType)&&((int)FULL_BODY_RECT == (int)targerType))
	{
		temp =  shouldToBody;
		//printf("shouldToBody\n");
	}
	destRect->left 	= srcRect->left - (UINT32)(temp[lr_aval] * (float)(srcRect->right - srcRect->left));

	//人脸框的时候如果在旁边，有可能扩放的时候数据不正确，如果框超出范围，就丢弃
	if(destRect->left > srcRect->left)
	{
		return -1;
		//destRect->left = 0;
	}
	destRect->top	= srcRect->top - (UINT32)(temp[top_off] *(float)(srcRect->bottom - srcRect->top));
	if(destRect->top > srcRect->top)
	{
		return -1;
		//destRect->top = 0;
	}
	destRect->right = srcRect->right + (UINT32)(temp[lr_aval]*(float)(srcRect->right - srcRect->left));
	if(destRect->right < srcRect->right)
	{
		return -1;
		//destRect->right = 0;
	}
	destRect->bottom = srcRect->bottom + (UINT32)(temp[bott_off]*(float)(srcRect->bottom - srcRect->top));
	if(destRect->bottom < srcRect->bottom)
	{
		return -1;
		//destRect->bottom = 0;
	}

	int backGoundPicWidth =  FrameManage::getInstance()->getFrameInfo()->Width;
	int backGoundPicHeight = FrameManage::getInstance()->getFrameInfo()->Height;
	if((destRect->right > backGoundPicWidth)||(destRect->left > backGoundPicWidth))
	{
		printf("ERR :destRect->right = %d,destRect->left = %d , backGoundPicWidth = %d\n",destRect->right,destRect->left,backGoundPicWidth);
		return -1;
	}
	if((destRect->bottom > backGoundPicHeight)||(destRect->top > backGoundPicHeight))
	{
		printf("ERR: destRect->top = %d,destRect->bottom = %d, backGoundPicHeight = %d\n",destRect->top, destRect->bottom,backGoundPicHeight);
		return -1;
	}
	return 0;
}

int EncodeModule::AddFaceEncodeQue(FaceNode *pFaceNode)
{
	//printf("AddFaceEncodeQue pFaceNode =%x\n",pFaceNode);
	if(NULL == pFaceNode)
	{
		return -1;
	}
	
	//printf("****AddFaceEncodeQue pFaceNode->TrackNo = %d\n",pFaceNode->TrackNo);
	m_FaceEncodeQue.push(*pFaceNode);
	PthreadManage::getInstance()->setFaceNodeSemPost();
	return 0;
}




//DetectCfg->BackgroundCtrl;
//背景图上传控制;
//高4位控制人脸框叠加，低4位控制背景压缩率，
//0x00：不上传背景图;
//0x01：压缩率25%的背景图;
//0x02：压缩率60%的背景图，
//0x04：压缩率90%的背景图;
//0x00（高4位0）：背景图不叠加人脸;0x10：背景图叠加人脸框
int EncodeModule::encodeImg(UINT8 * input, UINT8 *out, int startX, int startY, int width, int height,  int *outLen, int level, int type)
{
	int s32Ret = 0;

	int compressValue = level;

#ifndef X86COMPILE	
#ifndef HAISI_ENCODE_BY_USER

	HI_U32 *instream = (HI_U32 *)input;
	HI_U32 *outstream = (HI_U32 *)out;

	s32Ret =  HaisiEcode::getInstance()->initChnVENC( width, height, m_VencChn);//初始化通道
	if(s32Ret != 0)
	{
		printf("ERR:initChnVENC s32Ret = %d\n",s32Ret);
		return -1;
	}
	s32Ret =  HaisiEcode::getInstance()->setCompressValue(level, m_VencChn);
	if(s32Ret != 0)
	{
		printf("ERR:setCompressValue s32Ret = %d\n",s32Ret);
		return -1;
	}
	s32Ret =  HaisiEcode::getInstance()->setCropSize( startX, startY, width,  height , m_VencChn);//截图
	if(s32Ret != 0)
	{
		printf("ERR:setCropSize s32Ret = %d\n",s32Ret);
		return -1;
	}

	int backWidth =  FrameManage::getInstance()->getFrameInfo()->Width;
	int backHeight = FrameManage::getInstance()->getFrameInfo()->Height;

	s32Ret = HaisiEcode::getInstance()->haisiEncodeVENC(m_VencChn, 0, 0, backWidth, backHeight, instream, outstream,  outLen, compressValue);
	if( s32Ret != HI_SUCCESS )
	{
		printf("ERR: haisiEncodeVENC s32Ret = %d\n",s32Ret);
		return -1;
	}

#endif
#endif
	return s32Ret;	  
}


FaceEncodeQue & EncodeModule::getFaceEncodeQue()
{
	return m_FaceEncodeQue;
}

int EncodeModule::initEncodeModule(void *priData )
{
#ifndef X86COMPILE
#ifndef HAISI_ENCODE_BY_USER

	m_VencChn = 1;//初始化1
	HI_S32 s32Ret = HI_SUCCESS;
	int width =  FrameManage::getInstance()->getFrameInfo()->Width;
	int height = FrameManage::getInstance()->getFrameInfo()->Height;

  	HaisiEcode::getInstance()->initBaseHaisiEncode( width, height, m_VencChn);
	//printf("\n\n\n\n\n TEST init initBaseHaisiEncode,width = %d,\nhegiht =%d , m_VencChn = %d\n",width,height,m_VencChn);
#endif
#endif
	return 0;


}

int EncodeModule::uninitEncodeModule( )
{
	int ret32 = 0;
	if((int)(m_FaceEncodeQue.size())>0)
	{
		while (m_FaceEncodeQue.size()>0)
		{
			m_FaceEncodeQue.pop();
		}
	}
#ifndef X86COMPILE
#ifndef HAISI_ENCODE_BY_USER

	ret32 = HaisiEcode::getInstance()->uninitHaisiEncode(m_VencChn);
#endif
#endif

	return ret32;

}


EncodeModule::~EncodeModule()
{
}


EncodeModule::EncodeModule()
{
}

