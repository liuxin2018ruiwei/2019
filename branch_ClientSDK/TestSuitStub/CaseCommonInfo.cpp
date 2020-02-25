/********************************************************************
* Name      :  CaseCommonInfo.cpp                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  Zhang Botong                                          
* Date      :  2018-07-26                                                 
********************************************************************/

#include "CaseCommonInfo.h"
#include "TestDriverFactory.h"

CaseCommonInfo * CaseCommonInfo::m_this =  nullptr;

CaseCommonInfo * CaseCommonInfo::getInstance()
{
	if( nullptr == m_this )
	{
		m_this = new CaseCommonInfo;
	}

	return m_this;
}

int CaseCommonInfo::setCaseID( unsigned int caseID )
{
	m_caseID = caseID;

	return 0;
}

unsigned int CaseCommonInfo::getCaseID()
{
	return m_caseID;
}
CaseCommonInfo::~CaseCommonInfo()
{
}

CaseCommonInfo::CaseCommonInfo():m_caseID(TestDriverFactory::CaseIDMax)
{
}
