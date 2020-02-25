/********************************************************************
* Name      :  MsgProcessor.h                                  
* Describ   :  the class is used to process message received from port
* Author    :  Zhang Botong                                          
* Date      :  2018-06-06                                                 
********************************************************************/

#ifndef MSGPROCESSOR_H
#define MSGPROCESSOR_H

#include <map>
#include "Version.h"
#include "UartCommandCtrl.h"

class CommunicationAgent;
class MsgProcessor
{
 public: 
  typedef char InitInfo;

  static MsgProcessor* getInstance();

  int processMsg();

  int initMsgProcessor( InitInfo* priData );

  void uninitMsgProcessor();

  virtual ~MsgProcessor();
 protected:
  MsgProcessor();
 private:
  static MsgProcessor * m_this;
  CommunicationAgent *m_pComAgent;
  UartCommandCtrl *m_uartCMDCtrl;
};

#endif
