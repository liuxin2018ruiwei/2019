/********************************************************************
* Name      :  RequestProcessor.cpp                                  
* Describ   :                                                        
* Author    :  Zhang Botong                                          
* Date      :  2018-06-06                                                 
********************************************************************/

#include <stdio.h>
#include "RequestProcessor.h"
#include "RequestManage.h"
#include "ProcUartMsgReq.h"
#include "GetServiceVisionReq.h"
#include "SendHeartBeatReq.h"
#include "SwitchDetectorReq.h"
#include "SoftUpgradeReq.h"
#include "GetFaceDetecteVersionReq.h"
#include "SetBaudrateReq.h"



RequestProcessor *RequestProcessor::m_this = nullptr;


RequestProcessor* RequestProcessor::getInstance()
{
	if( nullptr == m_this )
	{
		m_this = new RequestProcessor;
	}

	return m_this;
}

int RequestProcessor::initRequestProcessor( InitInfo * pInfo )
{
#ifdef DEBUG_LOG
	printf("%d : %s\n", __LINE__, __func__);
#endif
	
	if( nullptr == pInfo )
	{
		printf("ERR: %d %s pInfo=%P\n", __LINE__, __func__, pInfo );
		return -1;
	}

	//接收收到的数据
	BaseReq * pReq = new ProcUartMsgReq;
	m_reqMap.insert(std::pair<Requestion::ReqID, BaseReq*>(pReq->getReqID(),pReq ) );

	//发送数据
	pReq = new GetServiceVisionReq;
	m_reqMap.insert(std::pair<Requestion::ReqID, BaseReq*>(pReq->getReqID(),pReq ) );
	
	pReq = new SendHeartBeatReq;
	m_reqMap.insert(std::pair<Requestion::ReqID, BaseReq*>(pReq->getReqID(),pReq ) );
	
	pReq = new SwitchDetectorReq;
	m_reqMap.insert(std::pair<Requestion::ReqID, BaseReq*>(pReq->getReqID(),pReq ) );

	pReq = new SoftUpgradeReq;
	m_reqMap.insert(std::pair<Requestion::ReqID, BaseReq*>(pReq->getReqID(),pReq ) );

	pReq = new GetFaceDetecteVersionReq;
	m_reqMap.insert(std::pair<Requestion::ReqID, BaseReq*>(pReq->getReqID(),pReq ) );

	pReq = new SetBaudrateReq;
	m_reqMap.insert(std::pair<Requestion::ReqID, BaseReq*>(pReq->getReqID(),pReq ) );
	
	return 0;
}

int RequestProcessor::unintRequestProcessor()
{
	for(ReqMapItr iter = m_reqMap.begin(); iter != m_reqMap.end();)
	{
		delete iter->second;
		iter = m_reqMap.erase(iter);
	}
	//m_reqMap.erase(m_reqMap.begin(), m_reqMap.end());
	//printf("[LX] unintRequestProcessor m_reqMap.size = %d \n",m_reqMap.size());
	return 0;
}

int RequestProcessor::processReq( )
{
	Requestion * pReq = RequestManage::getInstance()->getTopPriorityRequest();
	if( nullptr == pReq )
	{
		return 0;
	}
	
	Requestion::ReqID reqID = pReq->getRequestID();
	//printf("reqID = %x\n",reqID);
	

	int nRet = 0;
	ReqMapItr itr = m_reqMap.find( reqID );
	if( itr != m_reqMap.end() )
	{
		nRet = itr->second->processReq( pReq );
		//printf("reqID = %x\n",pReq->getRequestID());
	}
	else
	{
		nRet = -1;
	}
	
	if( pReq->getRequestState() == 0 )
	{
		//printf("____DELETE____\n\n");
		RequestManage::getInstance()->deleteRequest( pReq );
	}
	
	//printf("MsgReqProcPthd %d : %s,reqID = %d\n", __LINE__, __func__,reqID);

	return 0;	
}

RequestProcessor::~RequestProcessor()
{

}

RequestProcessor::RequestProcessor()
{

}
  
