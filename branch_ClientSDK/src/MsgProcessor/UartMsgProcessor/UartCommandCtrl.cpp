/********************************************************************
* Name      :  UartCommandCtrl.cpp                                  
* Describ   :  the difination of the class
* Author    :  Zhang Botong                                          
* Date      :  2018-06-15                                                 
********************************************************************/
#include <stdio.h>
#include "UartCommandCtrl.h"
#include "MessageQueue.h"
#include "UartMessageFilter.h"
#include "CommunicationAgent.h"

#include "QueryInfoUCmd.h"
#include "ActiveAppCfgUCmd.h"
#include "ActiveAppPkgUCmd.h"
#include "ActiveCamreaQPSIImageUCmd.h"
#include "GetFaceDetecteVersionUCmd.h"
#include "NotifyFaceInfoUCmd.h"
#include "HeartBeatUCmd.h"
#include "SoftUpgradeAppCfgCmd.h"
#include "SoftUpgradeAppPkgCmd.h"
#include "SoftUpgradeUCmd.h"
#include "StartFaceCheckUCmd.h"
#include "StopFaceCheckUCmd.h"



UartCommandCtrl::UartCommandCtrl()
{
	
}

int UartCommandCtrl::initUartCommandCtrl( InitInfo *pInfo )
{
	if( nullptr == pInfo )
	{
		return -1;
	}

	MessageFilterItf *baseFulter = new UartMessageFilter;
	CommunicationAgent::getInstance()->addFilter(baseFulter);
	//printf("CommunicationAgent -> addFilter ok \n");

	UartCommand *pCmd = new ActiveAppCfgUCmd;
	m_cmdMap.insert( std::pair< UartCommandCode::CodeType, UartCommand* >(pCmd->getCommandeCode(), pCmd ) );
	
	pCmd = new ActiveAppPkgUCmd;
	m_cmdMap.insert( std::pair< UartCommandCode::CodeType, UartCommand* >(pCmd->getCommandeCode(), pCmd ) );

	pCmd = new ActiveCamreaQPSIImageUCmd;
	m_cmdMap.insert( std::pair< UartCommandCode::CodeType, UartCommand* >(pCmd->getCommandeCode(), pCmd ) );
	
	pCmd = new GetFaceDetecteVersionUCmd;
	m_cmdMap.insert( std::pair< UartCommandCode::CodeType, UartCommand* >(pCmd->getCommandeCode(), pCmd ) );

	pCmd = new HeartBeatUCmd;
	m_cmdMap.insert( std::pair< UartCommandCode::CodeType, UartCommand* >(pCmd->getCommandeCode(), pCmd ) );

	pCmd = new NotifyFaceInfoUCmd;
	m_cmdMap.insert( std::pair< UartCommandCode::CodeType, UartCommand* >(pCmd->getCommandeCode(), pCmd ) );
	
	pCmd = new QueryInfoUCmd;
	m_cmdMap.insert( std::pair< UartCommandCode::CodeType, UartCommand* >(pCmd->getCommandeCode(), pCmd ) );
	
	pCmd = new SoftUpgradeAppCfgCmd;
	m_cmdMap.insert( std::pair< UartCommandCode::CodeType, UartCommand* >(pCmd->getCommandeCode(), pCmd ) );

	pCmd = new SoftUpgradeAppPkgCmd;
	m_cmdMap.insert( std::pair< UartCommandCode::CodeType, UartCommand* >(pCmd->getCommandeCode(), pCmd ) );

	pCmd = new SoftUpgradeUCmd;
	m_cmdMap.insert( std::pair< UartCommandCode::CodeType, UartCommand* >(pCmd->getCommandeCode(), pCmd ) );

	pCmd = new StartFaceCheckUCmd;
	m_cmdMap.insert( std::pair< UartCommandCode::CodeType, UartCommand* >(pCmd->getCommandeCode(), pCmd ) );


	pCmd = new StopFaceCheckUCmd;
	m_cmdMap.insert( std::pair< UartCommandCode::CodeType, UartCommand* >(pCmd->getCommandeCode(), pCmd ) );

	

	return 0;

}

void  UartCommandCtrl:: unInitUartCommandCtrl()
{
	m_cmdMap.erase( m_cmdMap.begin(),  m_cmdMap.end() );
}

int UartCommandCtrl::processMsg( Message * pMsg )
{
	if( nullptr == pMsg )
	{
		return -1;
	}
	
	UartCommandCode::CodeType code = pMsg->getCode();
	//printf("processMsg -->code = %x\n",code);
	int nRet = 0;
	CmdMapItr itr = m_cmdMap.find( code );
	if( itr != m_cmdMap.end() )
	{
		nRet = itr->second->processMsg( pMsg ); 
	}
	else
	{
		nRet = -1;
	}

	return nRet;

}

UartCommandCtrl::~UartCommandCtrl()
{
	
}
