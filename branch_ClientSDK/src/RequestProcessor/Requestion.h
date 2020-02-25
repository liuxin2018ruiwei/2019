/********************************************************************
* Name      :  Requestion.h                                  
* Describ   :  the class is used to save the information of a requestion
* Author    :  Zhang Botong                                          
* Date      :  2018-06-11                                                 
********************************************************************/

#ifndef REQUESTION_H
#define REQUESTION_H

#include "DetectFaceIFCommon.h"

class Requestion
{
public: 
  typedef enum
  {
	ProcUartMsgReq,
	ProcSDKMsgReq,
	ProcHttpMsgReq,
	GetServiceVisionReqID,
	SendHeartBeatConReq,
	SendSwitchDetectorReq,
	SoftUpgrade,
	GetFaceDetecteVersionReqID,
	SetBaudrate,
	ReqIDMax
  }ReqID;
  typedef enum
  {
	HighPriority,
	MiddlePriority,
	LowPriority
  }Priority;

  Requestion( ReqID id);
  Requestion( ReqID id,  Priority pro);

  ReqID getRequestID();

  UINT32 getRequestState();
  UINT32 setRequestState( UINT32 state );

  Priority getPriority();
  Priority setPriority( Priority pro );

  UINT32 getDestination();
  int setDestination( UINT32 dstID);

  UINT64 getTimer(); // unit: micro second
  virtual ~Requestion();
  
 protected:
 private:
  ReqID m_id;
  UINT64 m_timer;
  Priority m_pro;
  UINT32 m_state;
  UINT32 m_dstID;
};
#endif
