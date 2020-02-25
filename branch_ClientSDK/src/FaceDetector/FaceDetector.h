/********************************************************************
* Name      :  FaceDetector.h                                  
* Describ   :  the class is used to process face info from UART
* Author    :  Zhang Botong                                          
* Date      :  2018-06-06                                                 
********************************************************************/

#ifndef FACEDETECTOR_H
#define FACEDETECTOR_H

#include "Version.h"
#include <list>
#include "RegionJudger.h"
#include <FrameManage.h>
#include "BaseCaptureStratege.h"


using std::list;
#define MIN_POINT_NUM (3)
#define MAX_POINT_NUM (8)

#define HAS_FIND	   	(0)
#define HAS_NOT_FIND 	(1)


#define HAS_FIND_DETECTOR_REGION	 (1)
#define HAS_NOT_FIND_DETECTOR_REGION (0)

#define HAS_FIND_SHILED_REGION	   	(0)
#define HAS_NOT_FIND_SHILED_REGION 	(1)



typedef list<RegionJudger> RegionList;
typedef RegionList::iterator RegionListIter;

typedef list<BaseCaptureStratege *> StrategeList;
typedef StrategeList::iterator  StrategeListIter;


enum
{
	BACK_GROUND_MAP = 0,
	HEAD_HOULDER_FRAME = 1,
	HALF_BODY_PICTRUE_FRAME =2,
	BODY_PHOTO_FRAME = 3,
	FACE_FRAME = 4,
};

#define SET_FPGA_TO_CHANGE_BARUDRATE (1)
#define SEND_GET_SOFET_VERSION_AFTER_CHANGE_FPGA (2)
#define GET_DATA_AFTER_SEND_GET_VERION (3)
#define FLAG_NO_USE  (4)

class FaceDetector
{
 public: 
  static FaceDetector* getInstance();

  int initFaceDetector(void *priData );
  void uninitFaceDetector();
 
  
  int addDetectRegion( Point *pPoint, int nCount );
  
  int deletetDetectRegion( Point *pPoint, int nCount );

  int addShieldRegion( Point *pPoint, int nCount );
  
  int deletetShieldRegion (  Point *pPoint, int nCount );

  int releaseData();

  int countPeople(RWFaceInfo *pExtData);

  //filter face rule
  int CaptureThresholdFilter(RWFaceInfo *pExtData);
  
  int filterFace(RWFaceInfo *pExtData);

  int selectFace(RWFaceInfo *pExtData);

  int FaceSizeFilter(RWFaceInfo *pExtData);

  void setDetectType(UINT8 value);

  int scaleRect(RWFaceInfo *pSrc, RWFaceInfo *pDst );
  

  int inDetectRegion(Point point);


  RegionList & getlistRegion();
  
  RegionList& getListShieldRegion();


   
 StrategeList & getListStratege();

 char *  getUpdateFileName();

 int getFlagRecoverBarudrate();
 void setflagRecoverBarudrate(int data);

 long long getRecoverBarudTime();
 void setRecoverBarudTime(long long time);

 
 virtual ~FaceDetector();


 protected:
  FaceDetector();
 
  
 private:
  static FaceDetector* m_this;
  enum{DetectionRegionMax=4};
  RegionList m_listRegion;
  enum{DetectionShieldRegionMax=1};
  RegionList m_listShieldRegion;
  StrategeList m_pListStratege;
  char m_updateFileName[128];
  
  int m_flagRecoverBarudrate;//该参数是设置波特率失败后，恢复115200使用的。
  long long m_RecoverBarudTime;
};
#endif
