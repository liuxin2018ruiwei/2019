/********************************************************************
* Name      :  SpaceCaptureStratege.h                                  
* Describ   :  the class is used to  some valuse of heat beat
* Author    :  liuxin                                       
* Date      :  2018-07-9                                                
********************************************************************/

#ifndef SPACECAPTURESTRATEGE_H
#define SPACECAPTURESTRATEGE_H

#include "Version.h"
#include <FaceDataManage.h>
#include "BaseCaptureStratege.h"


typedef unsigned char UINT8;

class SpaceCaptureStratege:public BaseCaptureStratege
{
 public: 
 
  virtual int init(void *priData );
  virtual ~SpaceCaptureStratege();
  virtual int captureFace(FaceNode *pFaceNode);

 SpaceCaptureStratege();

};
#endif
