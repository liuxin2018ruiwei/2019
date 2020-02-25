/********************************************************************
* Name      :  HaisiEcode.h                                  
* Describ   :  
* Author    :  liuxin                                       
* Date      :  2018-06-25                                                 
********************************************************************/

#ifndef HAISI_ECODE_H
#define HAISI_ECODE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#include "hi_type.h"
#include "hi_comm_venc.h"
#include "hi_comm_vb.h"



#define FILE_NAME_LEN               128
#define MAX_FRAME_NUM 	(1)

class HaisiEcode
{
 public: 
 
  HI_S32 savePicData(char * saveDate, VENC_STREAM_S* pstStream, int *outLen);
  
  int setCompressValue(int compressValue,VENC_CHN VencChn );
  
  //int  SAMPLE_COMM_VENC_GetVencStreamProcEx(VENC_CHN VChn, int width, int height, HI_U32 *instream, HI_U32 *outstream, int compressValue);

  
 
  int HSStopGetStream();

  int HSVENCStop(VENC_CHN VencChn);

  int HSSYS_Exit();

  int initSendFrame(int width, int height, VENC_CHN VencChn);
  int getOutStream(HI_U32 *outstream, VENC_CHN VencChn, int width, int height, int *outLen);

  static HaisiEcode* getInstance();

  int initBaseHaisiEncode(int width,int height,VENC_CHN VencChn);
  
  int initHaisiEcode(void *priData );
  
  int haisiEncodeVENC(int VencChn, int x,int y, int width, int height, HI_U32* instream, HI_U32 * outstream, int *outLen, int compress );

  int setCropSize( int x, int y, int width, int height , int VencChn);
	
 /* int test1(int flag);
  int test();
  int test2(int flag);
  */

  int  initChnVENC(int width, int height,VENC_CHN VencChn);
  int  uninitHaisiEncode(VENC_CHN VencChn);
  int  haiSiSendfream (VENC_CHN VeChn);
   
 protected:
  HaisiEcode();
  virtual ~HaisiEcode();
  
 private:
 HI_VOID SAMPLE_COMM_ISP_Stop();
  static HaisiEcode* m_this;
  //int gs_IspPid;
  VIDEO_FRAME_INFO_S *m_pstFrame;
  VB_BLK  m_handleY;
  HI_U32 *m_pVirYaddr;
  HI_S32 m_VencFd;
  int m_width;
  int m_height;
  VENC_CHN m_vencChn;
 

};
#endif
