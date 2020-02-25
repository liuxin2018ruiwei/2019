/********************************************************************
* Name      :  SetBaudrateReq.h                                  
* Describ   : 
* Author    :  liuxin                                          
* Date      :  2018-06-25                                                 
********************************************************************/

#ifndef SET_BAUDRATE_REQUESTION_H
#define SET_BAUDRATE_REQUESTION_H

#include "Requestion.h"
#include "BaseReq.h"

typedef unsigned char UINT8;
typedef unsigned short int UINT16;



#define UART_SET_BAUDRATE_COMMAND			(0x0301)



class SetBaudrateReq:public BaseReq
{
 public: 
  SetBaudrateReq();
  virtual int processReq( Requestion *pReq );
  virtual ~SetBaudrateReq();
 protected:
 private:
};

#endif
