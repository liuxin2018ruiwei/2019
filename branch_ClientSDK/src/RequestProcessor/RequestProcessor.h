/********************************************************************
* Name      :  RequestProcessor.h                                  
* Describ   :  the class is used to process requestions from other module
* Author    :  Zhang Botong                                          
* Date      :  2018-06-06                                                 
********************************************************************/

#ifndef REQUESTPROCESSOR_H
#define REQUESTPROCESSOR_H

#include <map>
#include "Version.h"
#include "BaseReq.h"

class RequestProcessor
{
 public: 
  typedef char InitInfo;
  typedef std::map<Requestion::ReqID, BaseReq*> ReqMap;
  typedef ReqMap::iterator ReqMapItr;
  
  static RequestProcessor* getInstance();

  int initRequestProcessor( InitInfo * pInfo );
  int unintRequestProcessor();

  int processReq( );
  
  virtual ~RequestProcessor();
 protected:
  RequestProcessor();
  
 private:
  static RequestProcessor *m_this;
  ReqMap m_reqMap;
};
#endif
