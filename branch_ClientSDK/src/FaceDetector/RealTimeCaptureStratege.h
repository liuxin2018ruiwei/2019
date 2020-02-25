/********************************************************************
* Name      :  RealTimeCaptureStratege.h                                  
* Describ   :  the class is used to  some valuse of heat beat
* Author    :  liuxin                                       
* Date      :  2018-06-25                                                 
********************************************************************/

#ifndef REALTIMECAPTURESTRATEGE_H
#define REALTIMECAPTURESTRATEGE_H

#include "Version.h"
#include <FaceDataManage.h>
#include "BaseCaptureStratege.h"

typedef unsigned char UINT8;

class RealTimeCaptureStratege:public BaseCaptureStratege
{
 public: 
 
  virtual int init(void *priData );
  virtual ~RealTimeCaptureStratege();
  virtual int captureFace(FaceNode *pFaceNode);

 RealTimeCaptureStratege();

};
#endif
