/********************************************************************
* Name      :  IT_12_ModleShow_01.h                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  liuixn                                         
* Date      :  2018-07-23                                                 
********************************************************************/

#ifndef IT_12_ModleShow_01_H
#define IT_12_ModleShow_01_H

#include "Driver.h"




class IT_12_ModleShow_01:public Driver
{
	public: 
		
		IT_12_ModleShow_01( unsigned int nCaseId );
		virtual int init();
		virtual int run();
		int initDetectCfgInfo(int DetectThreshold);
		virtual int uninit();

		virtual ~IT_12_ModleShow_01();
		void initRegion();


		static void usrCallbackReplyCancalClient2(UINT64 *pReqID,
										   UINT8 *pRet,
										   UINT8 *pReplyData,
										   UINT32 *pDataLen );
  
	   static void usrCallbackReplyRegisterClient2(UINT64 *pReqID,
												 UINT8 *pRet,
												 UINT8 *pReplyData,
												 UINT32 *pDataLen );
	   static void usrCallbackNotifyServiceInfo2( RWNotifyType *pNotifyType,

	   UINT8 *pExtData,
												UINT32 *pDataLen);

	   static void usrCallbackNotifyFaceInfo2( RWNotifyType *pNotifyType,
											 UINT8 *pExtData,
											 UINT32 *pDataLen );

	   int init_initDetectFaceIF();

	   int  init_pCallBackInfo();

	   void setline();
	   void addRegion();
	   
	   void  myAddFrame();
	   void mySetDetectCfg();
	   void writeInt32(char* data, int value);	   
	   void initHeatBeat();
	   void initDetectFace();
	   int faceSize(UINT16 faceSizeValue);
	   int faceDetectCapture(int type);
	   
		int faceDetectThreshold(int value);
		void backGroundPicSwith(UINT8 data);
		int setFaceCtrl(UINT8 faceData);
		void sendQurySoftVersion();
	  	void qurySoftVersion();
	protected: 
	private:
	CallBackInfo callbackInfo;
	EncodeCallBackInfo encodeCallbackInfo;

	DetectConfigInfo cfgInfo;
	RWImgInfo imgInfo;
	SerialPortInfo serialInfo;
	RWIFInfo ifInfo;
	RWUserInfo userInfo;

	void * m_TFHandle;


};
#endif
