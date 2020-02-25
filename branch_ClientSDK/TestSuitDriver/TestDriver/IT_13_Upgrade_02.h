/********************************************************************
* Name      :  IT_13_Upgrade_02.h                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  liuixn                                         
* Date      :  2018-07-23                                                 
********************************************************************/

#ifndef IT_13_Upgrade_02_H
#define IT_13_Upgrade_02_H

#include "Driver.h"


class IT_13_Upgrade_02:public Driver
{
	public: 
		
		IT_13_Upgrade_02( unsigned int nCaseId );
		virtual int init();
		virtual int run();
		int initDetectCfgInfo(int DetectThreshold);
		virtual int uninit();

		virtual ~IT_13_Upgrade_02();
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
