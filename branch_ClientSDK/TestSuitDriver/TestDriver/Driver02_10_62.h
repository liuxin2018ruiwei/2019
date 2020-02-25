/********************************************************************
* Name      :  Driver02_10_62.h                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  Zhang Botong                                          
* Date      :  2018-07-02                                                 
********************************************************************/

#ifndef DRIVER02_10_62_H
#define DRIVER02_10_62_H

#include "Driver.h"

#define	 MASK_DETECT_TYPE 		(0x01)
#define  MASK_DETECT_HEART_BEAT (0x02) 
#define  MASK_DETECT_THRESHOLD 	(0x04)
#define	 MASK_BACK_GROUND_CTRL 	(0x08)
#define	 MASK_BACK_FACE_CTRL 	(0x10)
#define	 MASK_DETECT_TIME		(0x20)
#define	 MASK_FACE_SIZE			(0x40)




class Driver02_10_62:public Driver
{
public: 
enum
{ 
	CAMERA_CLOSE = 0,
	CAMERA_LEAVE_SNAP = 1,
	CAMERA_NOW_SNAP = 2,
	CAMERA_INTERVAL_SNAP = 3,
	CAMERA_LEAVE_NOW_SANP = 4
	  
};

//»À¡≥øÚ¿‡–Õ£∫0£∫±≥æ∞Õº, 1£∫Õ∑ºÁøÚ, 2£∫∞Î…Ì’’øÚ,3£∫»´…Ì’’øÚ, 4£∫»À¡≥øÚ
enum
{	
	BACK_GROUND_MAP = 0,	
	HEAD_HOULDER_FRAME = 1,
	HALF_BODY_PICTRUE_FRAME =2,	
	BODY_PHOTO_FRAME = 3,	
	FACE_FRAME = 4,
};
 Driver02_10_62( unsigned int nCaseId );
  virtual int init();
  virtual int run();
  int initDetectCfgInfo(int DetectThreshold);
  virtual int uninit();
  virtual ~Driver02_10_62();
  void initRegion();
 protected: 
 private:
 void lxTest();

};
#endif
