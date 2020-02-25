/********************************************************************
* Name      :  UartMessageFilter.h                                  
* Describ   :  the class is used to declare the interface
* Author    :  Zhang Botong                                          
* Date      :  2018-06-14                                                 
********************************************************************/

#ifndef UARTMESSAGEFILTER_H
#define UARTMESSAGEFILTER_H


#include "MessageQueue.h"

class UartMessageFilter:public MessageFilterItf
{
 public: 
  UartMessageFilter();

  virtual bool filter(Message* mes);
  virtual bool isExistCmdType( unsigned int cmdType,unsigned int cmdCode);


  virtual ~UartMessageFilter();
};
#endif
