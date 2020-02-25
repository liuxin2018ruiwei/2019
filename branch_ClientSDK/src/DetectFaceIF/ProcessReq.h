/********************************************************************
* Name      :  ProcessReq.h                                  
* Describ   :  the class is used to process request from user, and
               notify user result                  
* Author    :  Zhang Botong                                          
* Date      :  2018-06-07                                                 
********************************************************************/

#ifndef PROCESSREQ_H
#define PROCESSREQ_H

#include "Version.h"
#include "DetectFaceIFCommon.h"
#include "FaceDetector.h"
#include "RegionJudger.h"
#include "DetectFaceIF.h"
#include "UpgradeRequestion.h"


#define START_FACE_CHECK (1)
#define STOP_FACE_CHECK (0)
#define MAX_LINE_NUM (4)
#define MAX_UPGRAD_FILE_LEN (64)
#define MAX_UPGRAD_NAME_LEN (64)



class ProcessReq
{
 public: 
  static ProcessReq* getInstance();

  int getClientVersion( UINT8 *pVersion, int nLen );


  int getServiceSDKVersion( char * protocolVersion ,int len);


  int getFaceDetectVersionInfo( VersionInfo *pVersion);

  int initProcessReq( void * priData );
  void uninitProcessReq();
  
  int registerCallback( CallBackInfo * pCallbackFunc );
  int registerEncodeCallback(EncodeCallBackInfo *pEncodeCallbackInfo);

  int setDetectConfig( DetectCfg *pCfg );
  int setDetectType(DetectCfg *pCfg );
  int setHeartBeat( DetectCfg *pCfg );
  int setFaceSize(DetectCfg *pCfg );
  int setDetectTime(DetectCfg *pCfg );
  int setDetectThreshold(DetectCfg *pCfg );
  int setBackGroundCtrl( DetectCfg *pCfg );
  int setFaceCtrl( DetectCfg *pCfg );

  int getDetectConfig( DetectCfg *pCfg );
  int getDetectType(DetectCfg *pCfg);
  int getHeartBeat( DetectCfg *pCfg );
  int getFaceSize( DetectCfg *pCfg );
  int getDetectTime(DetectCfg *pCfg );
  int getDetectThreshold(DetectCfg *pCfg );
  int getBackGroundCtrl( DetectCfg *pCfg );
  int getFaceCtrl( DetectCfg *pCfg );
  
  int addDetectRegion( Point *pPoint, int nCount );
  int deletetDetectRegion( Point *pPoint, int nCount );

  int addShieldRegion( Point *pPoint, int nCount );
  int deletetShieldRegion (  Point *pPoint, int nCount );


  int sentStartUartFaceCheck();
  int sentStopUartFaceCheck();

  int startPthread();
  CallbackNotifyFunc getClientNotifyFun();
  CallbackReplyFunc  getUpgradeReplyFunC();
  CallbackNotifyFunc getNotifyFaceInfoFun();
  CallbackNotifyFunc getFuncCountPeopl();
  CallbackFaceEncodeFunc getFaceEncodeCallbackFunc();
  CallbackBackGroundEncodeFunc getBackGroundEncodeFunc();
  
  int addFrame( UINT8 *pImage );


  int startUpgrade( UINT64 *pReqID, UpgradeDataInfo *pDataInfo,CallbackReplyFunc pFunc);
 
  int notifToRestart();

  int isvalideLine( Line *line );
  int setLine( Line *pLine,CallbackNotifyFunc pFunc );
  int resetCounter();

  int PointInImgRect( Point *pPoint, int nCount );

  virtual ~ProcessReq();
 protected:
  ProcessReq();
 private:
  static ProcessReq *m_this;
  CallBackInfo m_callback;
  CallbackReplyFunc m_pCallbackUpgradeFunc;
  CallbackNotifyFunc m_pFuncCountPeople;
  CallbackFaceEncodeFunc m_pFaceEncodeCallbackFunc;
  CallbackBackGroundEncodeFunc m_pBackgroundEncodeCallFunc;
};
#endif
