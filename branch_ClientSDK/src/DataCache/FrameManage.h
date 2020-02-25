/********************************************************************
* Name      :  FrameManage.h                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  Zhang Botong                                          
* Date      :  2018-06-08                                                 
********************************************************************/

#ifndef FRAMEMANAGE_H
#define FRAMEMANAGE_H

#include "DetectFaceIFCommon.h"
#include <list>
#include "Version.h"



using std::list;

#ifndef HAISI_ENCODE_BY_USER
#define MAX_BACKG_GROUNF_QUEUE_SIZE (40)
#else
#define MAX_BACKG_GROUNF_QUEUE_SIZE (1)
#endif


typedef struct _backGruondPic
{
	UINT64 ID;
	UINT8* pic;
}BackGruondPic;

typedef struct _initImgInfo	
{	
  UINT32 Width; //图片的宽
  UINT32 Height; //图片的高
  UINT8 ImgType; // 图片的格式,取值 0：灰度图; 1：RGB888;2：NV21;3：Jpg(此时Step为0);4：Bmp;5:BGR888;6:YUV402P
  UINT32 ImgLen;// 图片数据的长度
}InitImgInfo;


typedef list<BackGruondPic *>  BackgrdPicList;
typedef BackgrdPicList::iterator BackPicListIter;

class FrameManage
{
public: 
	int addFrame( UINT8 *pImage );
	UINT8* getImage(UINT64 frameNumber);
	
	InitImgInfo *getFrameInfo();
	static FrameManage* getInstance();

	int initFrameMange(RWImgInfo * pFrameInfo );
	int uninitFrameMange();
	int releaseList();
	
	FrameManage();
	virtual ~FrameManage();
	protected:
	private:
	BackgrdPicList m_pImagList;
	BackgrdPicList m_pImagCacheList;
	static FrameManage* m_this;
	InitImgInfo *m_pInitImgInfo;
};
#endif
