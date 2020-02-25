/********************************************************************
* Name      :  LeftAreaCaptureStratege.h                                  
* Describ   :   
* Author    :  liuxin                                       
* Date      :  2018-06-25                                                 
********************************************************************/

#ifndef LEFTAREACAPTRUESTRATE_H
#define LEFTAREACAPTRUESTRATE_H

#include "Version.h"
#include <FaceDataManage.h>
#include "BaseCaptureStratege.h"

#define TIME_INTERVAL_LETF_CAPTURE	(1.8 * 1000)



typedef unsigned char UINT8;

class LeftAreaCaptureStratege:public BaseCaptureStratege
{
 public: 
 
  virtual int init(void *priData );
  virtual ~LeftAreaCaptureStratege();
  virtual int captureFace(FaceNode *pFaceNode);

 LeftAreaCaptureStratege();

};
#endif
