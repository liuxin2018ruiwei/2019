/********************************************************************
* Name      :  TestDriver.h                                  
* Describ   :                                                        
* Author    :  Zhang Botong                                          
* Date      :  2018-07-02                                                 
********************************************************************/

#ifndef DRIVER_H
#define DRIVER_H


#include "DetectFaceIFCommon.h"
//#include "DetectFaceIF.h"


class Driver
{
 public:
  Driver(unsigned int caseID );
  virtual int init();
  virtual int run();
  virtual int uninit();
  unsigned int getCaseID();
  void *getHandle();
  int setHandle(void* pHandle);
  virtual ~Driver();
 protected:
  static void usrCallbackReplyCancalClient(UINT64 *pReqID,
										   UINT8 *pRet,
										   UINT8 *pReplyData,
										   UINT32 *pDataLen );
  
  static void usrCallbackReplyRegisterClient(UINT64 *pReqID,
											 UINT8 *pRet,
											 UINT8 *pReplyData,
											 UINT32 *pDataLen );
  static void usrCallbackNotifyServiceInfo( RWNotifyType *pNotifyType,
											UINT8 *pExtData,
											UINT32 *pDataLen);

  static   void usrCallbackNotifyFaceInfo( RWNotifyType *pNotifyType,
										 UINT8 *pExtData,
										 UINT32 *pDataLen );
  int register_Client();
  int cancal_Client();
  int getServiceVersion();
  int getClientVersion();
  int add_Frame();

 private:
  void *m_pHandle;
  unsigned int m_caseID;
  UINT64 RequestID;
  const int ImgWidth;
  const int ImgHeight;
  char *DummyFrame;
  
};


#endif
