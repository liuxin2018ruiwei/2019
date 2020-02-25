/********************************************************************
* Name      :  PthreadManage.h                                  
* Describ   :  the class is used to manage the pthread
* Author    :  Zhang Botong                                          
* Date      :  2018-06-06                                                 
********************************************************************/

#ifndef PTHREADMANAGE_H
#define PTHREADMANAGE_H


class MsgReqProcPthd;
class EncodeImgPthd;
class SystemPthd;
class PthreadManage
{
 public: 
  static PthreadManage* getInstance();

  int initPthreadManage(void *priData );
  int uninitPthreadManage();
  
  int startPthread();
  int stopPthread();
  int setReqSemPost();
  int setFaceNodeSemPost();

virtual ~PthreadManage();

protected:
  PthreadManage();
 private:
  static PthreadManage* m_this;
  MsgReqProcPthd *m_pMsgReqProcPthd;
  EncodeImgPthd *m_pEncodeImgPthd;
  SystemPthd  *m_pSystemPthd;

};
#endif
