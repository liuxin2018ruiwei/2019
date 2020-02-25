/********************************************************************
* Name      :  GetFaceDetecteVersionUCmd.h                                  
* Describ   : 
* Author    :  liuxin                                         
* Date      :  2018-07-23                                                 
********************************************************************/

#ifndef GET_FACE_DETECTE_VERSION_CMD_H
#define GET_FACE_DETECTE_VERSION_CMD_H
#include "UartCommand.h"

#define PACKAGE_VERSION_LEN (64)
#define SYSTEME_VERSION_LEN (64)


class GetFaceDetecteVersionUCmd: public UartCommand
{
 public: 
  GetFaceDetecteVersionUCmd();
  virtual int processMsg(Message * pMsg);
  
  virtual ~GetFaceDetecteVersionUCmd();
 protected:
 private:
};
#endif
