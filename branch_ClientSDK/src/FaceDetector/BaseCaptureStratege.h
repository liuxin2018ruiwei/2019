/********************************************************************
* Name      :  BaseCaptureStratege.h                                  
* Describ   :   
* Author    :  liuxin                                       
* Date      :  2018-06-25                                                 
********************************************************************/

#ifndef BASECAPTURESTRATEGE_H
#define BASECAPTURESTRATEGE_H

#include "Version.h"
#include <FaceDataManage.h>





typedef unsigned char UINT8;

class BaseCaptureStratege
{
 public: 
	
  virtual int init(void *priData );
  virtual ~BaseCaptureStratege();
  virtual int captureFace(FaceNode *pFaceNode);
  BaseCaptureStratege();
};
#endif
