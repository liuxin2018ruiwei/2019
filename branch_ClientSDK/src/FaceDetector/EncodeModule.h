/********************************************************************
* Name      :  EncodeModule.h                                  
* Describ   :  
* Author    :  liuxin                                       
* Date      :  2018-06-25                                                 
********************************************************************/

#ifndef ENCODEMODULE_H
#define ENCODEMODULE_H

#include "Version.h"
#include <queue>
#include <FaceDataManage.h>
#include <string>
#include "EncodeModule.h"
#include "DetectCfgInfo.h"
#include "DrawRect.h"




using std::queue;
using std::string;


typedef unsigned char UINT8;

typedef queue<FaceNode> FaceEncodeQue;

//newType UINT8  RectType; //人脸框类型： 1：头肩框， 2：半身照框，3：全身照框， 4：人脸框
typedef enum _faceRectType
{
	FACE_SHOULD_RECT = 1,
	HALF_BODY_RECT =   2,
	FULL_BODY_RECT =   3,
	FACE_RECT 	   =   4,
}FaceRectType;


//typedef FaceEncodeQue::iterator FaceQueIter;

//typedef  int  ImgType;
//typedef  int  CompressLevel;

// ImgType 图片的格式
//0：灰度图; 1：RGB888;2：NV21;3：Jpg(Step=0);4：Bmp;5:BGR888;6:YUV402P
typedef enum _imageType
{
	GrayscaleMap = 0,
	RGB888 = 1,
	NV21 = 2,
	Jpg = 3,
	Bmp = 4,
	BGR888 = 5,
	YUV402P = 6
}ImgType;

typedef struct _faceAndImgInfoData
{
	RWFaceInfo faceInfoData;
	RWImgInfo  imgInfoData;
}FaceAndImgInfoData;

class EncodeModule
{
 public: 
  int processFaceInfo(FaceNode * pFaceNode);

  int AddFaceEncodeQue(FaceNode *pFaceNode);
  
  int scaleRect(Rect * destRect, Rect * srcRect, FaceRectType newFaceType, FaceRectType oldFaceType);

  int NotifyImgDataToUser(UINT8 *Image, int len,FaceNode *pFaceNode);

  //int getBGRFromYUV(UINT8 * pSrcGray, int width, int height,UINT8 * pDstGray, Rect *rect);
  
  int encodeImg(UINT8 * input, UINT8 *out,  int startX, int startY, int width, int height, int *outlen, int level, int type);
  
  FaceEncodeQue & getFaceEncodeQue();
  int changeFaceTypeToFaceRectType(FaceType & faceType);

  static EncodeModule* getInstance();
  int initEncodeModule(void *priData );
  
  int uninitEncodeModule();
  
  virtual ~EncodeModule();
 protected:
  EncodeModule();
  
 private:
  static EncodeModule* m_this;
  FaceEncodeQue m_FaceEncodeQue;
  DrawRect drawer;
  int m_VencChn;
};
#endif
