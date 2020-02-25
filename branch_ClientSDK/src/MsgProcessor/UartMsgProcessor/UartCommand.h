/********************************************************************
* Name      :  UartCommand.h                                  
* Describ   :  the class is used to declare the interface
* Author    :  Zhang Botong                                          
* Date      :  2018-06-14                                                 
********************************************************************/

#ifndef UARTCOMMAND_H
#define UARTCOMMAND_H

#include "MessageQueue.h"
#include "DetectFaceIFCommon.h"
#include "UartCommandType.h"

class UartCommand
{
 public: 
  UartCommand( UartCommandCode::CodeType code );

  virtual int processMsg( Message * pMsg );
  
  UartCommandCode::CodeType getCommandeCode();

  virtual ~UartCommand();
 protected:

 private:
  UartCommandCode::CodeType m_cmdCode;
};
#endif
