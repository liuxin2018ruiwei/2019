/********************************************************************
* Name      :  IT_14_CountPeople_05.h                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  liuixn                                         
* Date      :  2018-07-23                                                 
********************************************************************/

#ifndef IT_14_CountPeople_05_H
#define IT_14_CountPeople_05_H

#include "Driver.h"




class IT_14_CountPeople_05:public Driver
{
	public: 
		
		IT_14_CountPeople_05( unsigned int nCaseId );
		virtual int init();
		virtual int run();
		int initDetectCfgInfo(int DetectThreshold);
		virtual int uninit();

		virtual ~IT_14_CountPeople_05();
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

	DetectConfigInfo cfgInfo;
	RWImgInfo imgInfo;
	SerialPortInfo serialInfo;
	RWIFInfo ifInfo;
	RWUserInfo userInfo;

	void * m_TFHandle;

};
#endif