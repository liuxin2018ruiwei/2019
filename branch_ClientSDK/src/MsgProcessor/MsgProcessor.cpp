/********************************************************************
* Name      :  MsgProcessor.cpp                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  Zhang Botong                                          
* Date      :  2018-06-06                                                 
********************************************************************/
#include <stdio.h>
#include "MsgProcessor.h"
#include "CommunicationAgent.h"
#include "HeartBeatInfo.h"

MsgProcessor * MsgProcessor::m_this = nullptr;

MsgProcessor* MsgProcessor::getInstance()
{
	if( nullptr == m_this )
	{
		m_this = new MsgProcessor;
	}

	return m_this;
}

int MsgProcessor::initMsgProcessor( InitInfo * priData )
{
#ifdef DEBUG_LOG
	printf("%d : %s\n", __LINE__, __func__);
#endif	
	
	if( nullptr == priData )
	{
		printf("ERR: %d %s priData=%P\n", __LINE__, __func__, priData );
		
		return -1;
	}

	m_pComAgent = CommunicationAgent::getInstance();

	m_uartCMDCtrl =  new UartCommandCtrl;
	UartCommandCtrl::InitInfo info;
	m_uartCMDCtrl->initUartCommandCtrl( &info );
	
	return 0;
   

}

int MsgProcessor::processMsg()
{
	int ret32 = 0;
	Message* pMsg = m_pComAgent->receiveMsg();

	//printf("\nget pMsg = %x \n\n",pMsg);

	if( NULL == pMsg )
	{
		return -1;
	}

	ProtocolType protclType = pMsg->getFromProtocol();
	//printf(" protclType = %d \n\n",protclType);
	if( UART_Protocol == protclType )
	{
		ret32 = m_uartCMDCtrl->processMsg( pMsg );
	}
	else if( SDK_Protocol == protclType )
	{
		// TODO: m_sdkCMDCtrl->processMsg( pMsg );
	}
	else if( Http_Protocol == protclType )
	{
		// TODO: m_httpCMDCtrl->processMsg( pMsg );
	}
	else
	{
		// NOP
	}

	//接收到数据将函数心跳重置，最后收到回复时间更新
	if(0 == ret32)
	{
		HeartBeatInfo::getInstance()->getNowTimeSetLastReplyTime();
		HeartBeatInfo::getInstance()->setHeatBeatLostCount(0);
	}
	
	m_pComAgent->releaseMsg( pMsg );
	
	return 0;
}

void MsgProcessor::uninitMsgProcessor()
{
	m_uartCMDCtrl->unInitUartCommandCtrl();
}


MsgProcessor::~MsgProcessor()
{
	
}

MsgProcessor::MsgProcessor():m_pComAgent( nullptr ),m_uartCMDCtrl( nullptr )
{
	
}
