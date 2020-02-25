/********************************************************************
* Name      :  IT_09_BackPicCompress_04.h                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  liuixn                                         
* Date      :  2018-07-23                                                 
********************************************************************/

#ifndef IT_09_BackPicCompress_04_H
#define IT_09_BackPicCompress_04_H

#include "Driver.h"




class IT_09_BackPicCompress_04:public Driver
{
	public: 
		
		IT_09_BackPicCompress_04( unsigned int nCaseId );
		virtual int init();
		virtual int run();
		int initDetectCfgInfo(int DetectThreshold);
		virtual int uninit();

		virtual ~IT_09_BackPicCompress_04();
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
