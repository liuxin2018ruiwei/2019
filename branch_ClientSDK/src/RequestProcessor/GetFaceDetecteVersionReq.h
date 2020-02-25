/********************************************************************
* Name      :  GetFaceDetecteVersionReq.h                                  
* Describ   : 
* Author    :  liuxin                                          
* Date      :  2018-06-25                                                 
********************************************************************/

#ifndef GET_FACE_DETECTE_VERSION_REQUESTION_H
#define GET_FACE_DETECTE_VERSION_REQUESTION_H

#include "Requestion.h"
#include "BaseReq.h"

typedef unsigned char UINT8;
typedef unsigned short int UINT16;


#define UART_GET_FACE_VERSION_FLAG_SEND_BIT		(0x00)
#define UART_GET_FACE_VWESION_FLAG_REC_BIT  	(0x80)
#define UART_GET_FACE_VERSION_FLAG_REPORT_BIT	(0x10)
#define UART_GET_FACE_VERSION_FLAG_UPGRADE_BIT	(0xC0)
#define UART_GET_FACE_VERSION_COMMAND			(0x0105)
#define UART_GET_FACE_VERSION_COMMAND_LEN		(2)
#define UART_GET_FACE_VERSION_SEND_LEN			(4)
#define UART_GET_FACE_VERSION_COMMAND_LOCATION 	(0)
#define UART_GET_FACE_VERSION_FLAG_LOCATION		(2)
#define UART_GET_FACE_VERSION_SEQUENCE_NUMBER_LOCATION (3)


class GetFaceDetecteVersionReq:public BaseReq
{
 public: 
  GetFaceDetecteVersionReq();
  virtual int processReq( Requestion *pReq );
  virtual ~GetFaceDetecteVersionReq();
 protected:
 private:
};

#endif
