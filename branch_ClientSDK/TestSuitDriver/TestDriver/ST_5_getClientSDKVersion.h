/********************************************************************
* Name      :  ST_5_getClientSDKVersion.h                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  liuixn                                         
* Date      :  2018-07-23                                                 
********************************************************************/

#ifndef ST_5_getClientSDKVersion_H
#define ST_5_getClientSDKVersion_H

#include "Driver.h"


class ST_5_getClientSDKVersion:public Driver
{
	public: 
		
		ST_5_getClientSDKVersion( unsigned int nCaseId );
		virtual int init();
		virtual int run();
		int initDetectCfgInfo(int DetectThreshold);
		virtual int uninit();

		virtual ~ST_5_getClientSDKVersion();
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
	protected: 
	private:
	CallBackInfo callbackInfo;

	DetectConfigInfo cfgInfo;
	RWImgInfo imgInfo;
	SerialPortInfo serialInfo;
	RWIFInfo ifInfo;
	RWUserInfo userInfo;

	void * m_TFHandle;

};
#endif
