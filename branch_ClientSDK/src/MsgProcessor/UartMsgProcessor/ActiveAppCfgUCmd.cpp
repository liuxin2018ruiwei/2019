/********************************************************************
* Name      :  ActiveAppCfgUCmd.cpp                                  
* Describ   :  
* Author    :  liuxin                                        
* Date      :  2018-06-26                                                 
********************************************************************/
#include <stdio.h>
#include "ActiveAppCfgUCmd.h"
#include "UartCommandType.h"
#include "LittleEndianIO.h"
#include "ProcessReq.h"
#include "SoftUpgradeAppPkgCmd.h"


ActiveAppCfgUCmd::ActiveAppCfgUCmd():UartCommand( UartCommandCode::QUERY_ACTIVE_APP_CFG ) 
{
	
}

int ActiveAppCfgUCmd::processMsg(Message * pMsg )
{
	
	if(NULL == pMsg)
	{
		return -1;
	}

	UINT8  flags = pMsg->getFlags(); //Flags
	if( (flags ==UART_SOFT_UPGRADE_REC_BIT) && ((flags & 0x01) == 0) )
	{
		ProcessReq::getInstance()->notifToRestart();
		return 0;
	}
	
	return 0;
}
ActiveAppCfgUCmd::~ActiveAppCfgUCmd()
{
}
