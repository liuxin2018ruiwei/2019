/********************************************************************
* Name      :  UartCommand.cpp                                  
* Describ   :  the defination of the class
* Author    :  Zhang Botong                                          
* Date      :  2018-06-14                                                 
********************************************************************/

#include<UartCommand.h>

UartCommand::UartCommand( UartCommandCode::CodeType code ):m_cmdCode( code )
{
}

int UartCommand::processMsg( Message * pMsg )
{
	return 0;
}

UartCommandCode::CodeType UartCommand::getCommandeCode()
{
	return m_cmdCode;
}

UartCommand::~UartCommand()
{
}
