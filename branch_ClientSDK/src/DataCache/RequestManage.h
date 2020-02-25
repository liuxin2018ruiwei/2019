/********************************************************************
* Name      :  RequestManage.h                                  
* Describ   :  the class is used to manage the requestion from any module,
*              offer the methode to read and write. and make sure only one
*              operation at the same time
* Author    :  Zhang Botong                                          
* Date      :  2018-06-08                                                 
********************************************************************/

#ifndef REQUESTMANAGE_H
#define REQUESTMANAGE_H

#include<list>
#include "Requestion.h"
#include "RecoLock.h"

class RequestManage
{
 public:
  enum{RequestTimeOut=500000};// 500 millisecond
  typedef std::list< Requestion* > ReqList;
  typedef ReqList::iterator ReqItr;

  // delete a requestion from the unique
  int deleteRequest( Requestion * pReq );
  static RequestManage * getInstance();
  // get a requestion from the unique of designated priority 
  Requestion * getRequest(Requestion::Priority pro );
  // get the requestion of highest priority from all of requestion unique
  Requestion * getTopPriorityRequest();

  // set a requestion to the unique
  int setRequest( Requestion *pReq );
  
  virtual ~RequestManage();
 protected:
  RequestManage();
  int deleteRequestFromList( ReqList &proList, Requestion *pReq );
 private:
  static RequestManage * m_this;
  ReqList m_highProList;
  RecoLock	m_hpListlock;
  ReqList m_middleProList;
  RecoLock	m_mpListlock;
  ReqList m_lowProList;
  RecoLock	m_lpListlock;
};
#endif
