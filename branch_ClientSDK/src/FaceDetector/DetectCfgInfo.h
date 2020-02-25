/********************************************************************
* Name      :  DetectCfgInfo.h                                  
* Describ   :  获取或者设置DetectCfg这个结构体变量（除了心跳和掩码）
* Author    :  liuxin                                       
* Date      :  2018-06-25                                                 
********************************************************************/

#ifndef DETECTCGFINFO_H
#define DETECTCGFINFO_H

#include "Version.h"


#define	 MASK_DETECT_TYPE 		(0x01)
#define  MASK_DETECT_HEART_BEAT (0x02) 
#define  MASK_DETECT_THRESHOLD 	(0x04)
#define	 MASK_BACK_GROUND_CTRL 	(0x08)
#define	 MASK_FACE_CTRL 		(0x10)
#define	 MASK_DETECT_TIME		(0x20)
#define	 MASK_FACE_SIZE			(0x40)

#define	 DEFUTE_DETECT_TIME 	(5)
#define  ERR_FACE_COMPRESS_LEVEL (0x08)


typedef unsigned char UINT8;
typedef unsigned short int UINT16;
typedef unsigned int UINT32;


enum
{
	MIN_FACE_SIZE = 0,
	MAX_FACE_SIZE = 512
 };

enum
{
	MIN_DETECT_TIME_SIZE = 0,
	MAX_DETECT_TIME_SIZE = 60
};

//DetectThreshold
enum
{
	MIN_DETECT_THRESHOLD_SIZE = 0,
	MAX_DETECT_THRESHOLD_SIZE = 100
};

//BackgroundCtrl
enum
{
	MIN_BACK_GROUND_CTRL = 0,
	MAX_BACK_GROUND_CTRL = 255
};

//FaceCtrl
enum
{
	MIN_FACE_CTRL = 0,
	MAX_FACE_CTRL = 255
};
//FaceCtrl
///人脸图控制;（高4位控制人脸图类型，低4位控制人脸图压缩率）0x10：人脸图;0x20 头肩照;0x40 半身照; 0x80 全身照;0x01压缩率25&的人脸图，0x02压缩率60%的人脸图;0x04压缩率90%的人脸图
typedef enum _faceType
{
	FACE_MAP = 0x01,
	HEAD_SHOULDER_PHOTOGRAPH = 0x02,
	HALF_BODY = 0x04,
	FULL_BODY_PICTRUE = 0x08,
}FaceType;

typedef enum _compressLevel
{
	COMPRESSION_25_FACE_MAP = 0x10,
	COMPRESSION_60_FACE_MAP = 0x20,
	COMPRESSION_90_FACE_MAP = 0x40
}CompressLevel;

//BackgroundCtrl
//背景图上传控制;高4位控制人脸框叠加，低4位控制背景压缩率，0x00：不上传背景图，0x01：压缩率25%的背景图;0x02：压缩率60%的背景图，0x04：压缩率90%的背景图;0x00（高4位0）：背景图不叠加人脸;0x10：背景图叠加人脸框
typedef enum _backgroundCompressLevel
{
	NOT_UPLOAD_BACK_GROUND_MAP = 0x00,
	COMPRESSION_25_BACK_GROUND_MAP = 0x01,
	COMPRESSION_60_BACK_GROUND_MAP = 0x02,
	COMPRESSION_90_BACK_GROUND_MAP = 0x04,
}BackgroundCompressLevel;

typedef enum  _backGroundSuperstack
{
	BACKGROUND_IMAGE_DO_NOT_SUPERSTACK_FACE_FRAME = 0x00,
	BACKGROUND_IMAGE_SUPERSTACK_FACE_FRAME = 0x10,
}BackGroundSuperstack;




//抓拍类型，0：关闭抓拍，1：离开后抓拍（默认值），2：实时抓拍，3：间隔抓拍， 4：离开后+实时抓拍。
enum
{ 
	CAMERA_CLOSE = 0,
	CAMERA_LEAVE_SNAP = 1,
	CAMERA_NOW_SNAP = 2,
	CAMERA_INTERVAL_SNAP = 3,
	CAMERA_LEAVE_NOW_SANP = 4
	  
};

typedef struct _Rect
{
	UINT32 left; 	// 
	UINT32 right; 	// 
	UINT32 top; 	//  
	UINT32 bottom; 	//  
}Rect;


class DetectCfgInfo
{
 public: 
  UINT8 getDetectType();	
  void setDetectType(UINT8 value);
  
  UINT8 getDetectThreshold();	
  void setDetectThreshold(UINT8 value);

  
  UINT8 getBackGroundCtrl();	
  void setBackGroundCtrl(UINT8 value);

  UINT8 getFaceCtrl();	
  void setFaceCtrl(UINT8 value);
  
  UINT8 getDetectTime();	
  void setDetectTime(UINT8 value);
  
  UINT16 getFaceSize();
  void setFaceSize(UINT16 value);

  int getSentFlag();
  void setSentFlag(int value);

  UINT8 getFaceType();
  UINT8 getFaceCompressLevel();
  UINT8 getBackGrundCompressLevel();
  UINT8 getBackGrundSuperstack();
  
  
  static DetectCfgInfo* getInstance();
  int initDetectCfgInfo(void *priData );
  virtual ~DetectCfgInfo();
  
	

 protected:
  DetectCfgInfo();
  
 private:
  static DetectCfgInfo* m_this;
	//UINT32 m_configMask;//掩码，只有标志位为1的对应项有效。标识为0的项目忽略
	UINT8 m_detectType;//抓拍类型，0：关闭抓拍，1：离开后抓拍（默认值），2：实时抓拍，3：间隔抓拍， 4：离开后+实时抓拍。
	//UINT8 DetectHeartbeat;//心跳间隔时长，0：关闭对检测模块的心跳检测，正值：心跳检测间隔（默认为10秒）
	UINT8 m_detectThreshold;//抓拍阈值，大于抓拍阈值的人脸才会考虑。范围0-100.默认值 1; 0表示检测到的头肩结果也考虑
	UINT8 m_backgroundCtrl;//背景图上传控制;高4位控制人脸框叠加，低4位控制背景压缩率，0x00：不上传背景图，0x01：压缩率25%的背景图;0x02：压缩率60%的背景图，0x04：压缩率90%的背景图;0x00（高4位0）：背景图不叠加人脸;0x10：背景图叠加人脸框
	UINT8 m_faceCtrl;//人脸图控制;（高4位控制人脸图类型，低4位控制人脸图压缩率）0x01：人脸图;0x02 头肩照;0x04 半身照; 0x08 全身照;0x10压缩率25&的人脸图，0x20压缩率60%的人脸图;0x40压缩率90%的人脸图
	UINT8 m_detectTime;//在实时抓拍，表示人脸出现X秒后抓拍；间隔抓拍中，表示每隔X秒抓拍一张。
	UINT16 m_faceSize; //最小人脸;小于最小人脸的数据会别忽略。取值0-512. 默认值0
	int m_sentUartFaceCheckFlag;  
	UINT8 m_faceType;
	UINT8 m_faceCompressLevel;
	UINT8 m_backGroundSuperstack;
	UINT8 m_backGrCompLevel;
};
#endif
