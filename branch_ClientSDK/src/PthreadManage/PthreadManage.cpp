/********************************************************************
* Name      :  PthreadManage.cpp                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  Zhang Botong                                          
* Date      :  2018-06-06                                                 
********************************************************************/

#include <stdio.h>
#include "PthreadManage.h"
#include "MsgReqProcPthd.h"
#include "EncodeImgPthd.h"
#include "SystemPthd.h"
#include "Communication.h"

PthreadManage* PthreadManage::m_this = nullptr;
PthreadManage* PthreadManage::getInstance()
{
	if( nullptr == m_this )
	{
		m_this = new PthreadManage;
	}

	return m_this;
}

int PthreadManage::initPthreadManage(void *priData )
{
	if( nullptr == priData )
	{
		printf("ERR %d %s priData=%P\n", __LINE__, __func__, priData );
		return -1;
	}

	m_pMsgReqProcPthd = new MsgReqProcPthd;
	MsgReqProcPthd::InitInfo info1;
	int nRet = m_pMsgReqProcPthd->initWithInfo( &info1 );

	m_pEncodeImgPthd = new EncodeImgPthd;
	EncodeImgPthd::InitInfo info2;
	nRet |= m_pEncodeImgPthd->initWithInfo( &info2 );

	
	m_pSystemPthd = new SystemPthd;
	SystemPthd::InitInfo info3;
	nRet |= m_pSystemPthd->initWithInfo( &info3);

	return nRet;
}

int PthreadManage::setReqSemPost()
{
	return m_pMsgReqProcPthd->setReqSemPost();
}

int PthreadManage::setFaceNodeSemPost()
{
	return m_pEncodeImgPthd->setFaceNodeSemPost();
}

int PthreadManage::startPthread()
{
	//Communication::getInstances()->start();

	m_pEncodeImgPthd->start();
	
	m_pSystemPthd->start();

	m_pMsgReqProcPthd->start();
	//printf("m_pMsgReqProcPthd = %x start \n",m_pMsgReqProcPthd);
	

	return 0;
}

int PthreadManage::stopPthread()
{
	Communication::getInstances()->stop();
	//printf("Communication  stop \n");
	
	m_pSystemPthd->stop();
	//printf("m_pSystemPthd = %x stop \n",m_pSystemPthd);


	m_pMsgReqProcPthd->stop();
	//printf("m_pMsgReqProcPthd = %x stop \n",m_pMsgReqProcPthd);


	m_pEncodeImgPthd->stop();
	//printf("m_pEncodeImgPthd = %x stop \n",m_pEncodeImgPthd);
	return 0;
}

int PthreadManage::uninitPthreadManage()
{
	Communication::getInstances()->uninit();

	m_pMsgReqProcPthd->uninit();
	m_pEncodeImgPthd->uninit();
	m_pSystemPthd->uninit();

	return 0;
}

PthreadManage::~PthreadManage()
{
}

PthreadManage::PthreadManage(): m_pMsgReqProcPthd(nullptr ),
								m_pEncodeImgPthd(nullptr)
{
}
