/********************************************************************
* Name      :  IT_13_Upgrade_03.h                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  liuixn                                         
* Date      :  2018-07-23                                                 
********************************************************************/

#ifndef IT_13_Upgrade_03_H
#define IT_13_Upgrade_03_H

#include "Driver.h"


class IT_13_Upgrade_03:public Driver
{
	public: 
		
		IT_13_Upgrade_03( unsigned int nCaseId );
		virtual int init();
		virtual int run();
		int initDetectCfgInfo(int DetectThreshold);
		virtual int uninit();

		virtual ~IT_13_Upgrade_03();
		void initRegion();


		static void usrCallbackReplyCancalClient1(UINT64 *pReqID,
										   UINT8 *pRet,
										   UINT8 *pReplyData,
										   UINT32 *pDataLen );
  
	   static void usrCallbackReplyRegisterClient1(UINT64 *pReqID,
												 UINT8 *pRet,
												 UINT8 *pReplyData,
												 UINT32 *pDataLen );
	   static void usrCallbackNotifyServiceInfo1( RWNotifyType *pNotifyType,

	   UINT8 *pExtData,
												UINT32 *pDataLen);

	   static void usrCallbackNotifyFaceInfo1( RWNotifyType *pNotifyType,
											 UINT8 *pExtData,
											 UINT32 *pDataLen );

	   int init_initDetectFaceIF();
	   int  init_pCallBackInfo();
	   int getpDataCfgInfo( );
	   
	   int upgradeAppCfg();
	   int upgradeAppPkg();
	   int upgradeCamereaQSPI();
	   void qurySoftVersion();
	   void initHeatBeat();

	   
	   void mySetDetectCfg();
	   void setline();
	   int faceSize(UINT16 faceSizeValue);
	   void initDetectFace();
	   void addRegion();
	   int  faceDetectCapture(int type);
		
		int setFaceCtrl(UINT8 faceData);
	   void backGroundPicSwith(UINT8 data);
	   void  myAddFrame();
	   void writeInt32(char* data, int value);
	   int faceDetectThreshold(int value);
	   
	protected: 
	private:
	CallBackInfo callbackInfo;

	DetectConfigInfo cfgInfo;
	RWImgInfo imgInfo;
	SerialPortInfo serialInfo;
	RWIFInfo ifInfo;
	RWUserInfo userInfo;

	void * m_TFHandle;
	UpgradeDataInfo m_upgradeDataInfo ;

};
#endif
