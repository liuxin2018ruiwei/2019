/********************************************************************
* Name      :  ConfigManage.h                                  
* Describ   :  the class is used to manage the configuration about the sdk
               and the state of the other module
* Author    :  Zhang Botong                                          
* Date      :  2018-06-05                                                 
********************************************************************/

#ifndef CONFIGMANAGE_H
#define CONFIGMANAGE_H

#include "DetectFaceIFCommon.h"

class ConfigManage
{
#define DETECT_TYPE_LOAD_SDK (0x01)
#define DETECT_TYPE_UNLOAD_SDK (0x00)
#define DEFAULT_BAUDRATE_FPGA (115200)


 public: 
  enum
  {
	DetectType_OFF,
	DetectType_ON
  };
  
  static ConfigManage * getInstance();
  virtual ~ConfigManage();
  int initConfigManage( DetectConfigInfo* pConfig );
  int uninitConfigManage();

  int getModuleProtocolVersion( char *pVersion, int nLen );
  int setModuleProtocolVersion( char * protocolVersion, int nLen );

  int getModuleSoftwareVersion( char *pVersion, int nLen );
  int setModuleSoftwareVersion( char * softwareVersion, int nLen );

  int getModuleFirmwareVersion( char *pVersion, int nLen );
  int setModuleFirmwareVersion( char * firmwareVersion, int nLen );

  UINT64 getCameraID();

  int setBaudrate(UINT32 baudrateData);
  int initGetSoftVersion();
  UINT32 getBaudRate();
  int  forkNew();
  int  updateApp(char * data, int type);
  char* getUartDevPtr();
  int getUartDevLen();
  int reOpenUart();
  int sendToUpdata(int uartHand , int type,char *fileName );
  
  
 protected:
  ConfigManage();

 private:
  static ConfigManage *m_this;
  UINT8 m_detectType;
  UINT8 m_transferType;
  UINT64 m_cameraID;
  VersionInfo m_moduleVersion;
  UINT32 m_baudrate;
  UINT8 m_DetectType;
  int fd[2];
  UINT8 m_DevData[64];
  UINT8 m_devLen;
  int m_semid;
  int m_semid2;
};
#endif
