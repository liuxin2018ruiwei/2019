/********************************************************************
* Name      :  Driver02_10_115.h                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  Zhang Botong                                          
* Date      :  2018-07-02                                                 
********************************************************************/

#ifndef DRIVER02_10_115_H
#define DRIVER02_10_115_H

#include "Driver.h"

#define   APP_CFG_UPGRADE_NAME  "app.cfg"
#define   APP_PKG_UPGRADE_NAME "app.pkg"
#define	  CAMERA_QSPI_IMAGE_UPGRADE_NAME "Camera-QSPI-image.bin"


class Driver02_10_115:public Driver
{
public: 

 Driver02_10_115( unsigned int nCaseId );
  virtual int init();
  virtual int run();
  int initDetectCfgInfo(int DetectThreshold);
  virtual int uninit();
 // void testTest(UINT64 *pReqID, UINT8 *pRet, UINT8 *pReplyData,UINT32 *pDataLen );
  void writeInt32(UINT8* data, int value);
  virtual ~Driver02_10_115();
  void initRegion();
 protected: 
 private:
 void lxTest();

};
#endif
