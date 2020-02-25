/********************************************************************
* Name      :  CaseCommonInfo.h                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  Zhang Botong                                          
* Date      :  2018-07-26                                                 
********************************************************************/

#ifndef CASECOMMONINFO_H
#define CASECOMMONINFO_H

class CaseCommonInfo
{
public:
  static CaseCommonInfo * getInstance();

  int setCaseID( unsigned int caseID );

  unsigned int getCaseID();
  
  virtual ~CaseCommonInfo();
protected:
 CaseCommonInfo();
private:
 static CaseCommonInfo * m_this;
 unsigned int m_caseID;
};
#endif
