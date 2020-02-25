/********************************************************************
* Name      :  ImgName.h                                  
* Describ   :  
* Author    :  liuxin                                        
* Date      :  2018-07-24                                               
********************************************************************/

#ifndef DATACACHE_H
#define DATACACHE_H

#include <FaceDataManage.h>


typedef unsigned long long UINT64;

class ImgName
{
 public: 
 	typedef enum _imgaNameType
	{
		FACE_IMAGE_NAME = 0,
		BACKGROUND_IMG_NAME = 1
	}ImageNameType;
  static ImgName * getInstance();

  int initImgName( void *priData );

  int getImagName( char *outName, int *outLen, int inputLen,FaceNode * faceNode, ImageNameType flag );

  //Face_���id_����id_ʱ���.jpg
  int getFaceImagName( char *outName, int *outLen,int inputLen, FaceNode * faceNode );
  //TG_���id_����id_ʱ���.jpg
  int getBackgroundImgName(char * outName, int *outLen,int inputLen, FaceNode * faceNode);
	
  virtual ~ImgName();
 protected:
  ImgName();
 private:
  static ImgName * m_this;

};
#endif
