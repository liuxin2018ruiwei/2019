#include <stdio.h>
#include "CommunicationAgent.h"
//#include "log.h"
#include "file_util.h"
#include "Version.h"
#include "RequestManage.h"
#include "Communication.h"


/*******************************
创建实例,需要调用destroyInstance释放
com 					: 通信类指针
receiveQueueSize		: 接收的消息队列长度
responseDataLength		: 发送消息缓冲，设置为需要发送的消息的最大数据长度
成功返回实例指针，失败返回0
*********************************/
CommunicationAgent * CommunicationAgent::_instance = NULL;
CommunicationAgent* 
CommunicationAgent::createInstance(Communication* com,
								   unsigned int receiveQueueSize,
								   unsigned int responseDataLength)
{
	CommunicationAgent* agent = new CommunicationAgent(com);
	if( 0 != agent->init(receiveQueueSize,responseDataLength) )
	{
		delete agent;
		return NULL;
	}

	return agent;
}

			
CommunicationAgent * CommunicationAgent::getInstance()
{
	if( NULL == _instance )
	{
		_instance = createInstance( Communication::getInstances(),
									RECEIVE_MESSAGE_QUEUE_SIZE,
									RECEIVER_MESSAGE_BUFF_SIZE);
	}

	return _instance;
}


void CommunicationAgent::destroyInstance(CommunicationAgent** agent)
{
	if( NULL == agent )
		return ;
	if( NULL == *agent )
		return ;
	(*agent)->uninit();
	delete (*agent);
	*agent = NULL;
}


CommunicationAgent::CommunicationAgent(Communication* com)
		: 	_com(com),
			_sendCache(NULL)
{
	
}


int CommunicationAgent::init(unsigned int receiveQueueSize,
							 unsigned int responseDataLength)
{
#ifdef DEBUG_LOG
		printf("%d : %s\n", __LINE__, __func__);
#endif	

	_sendCache = Message::createInstance(responseDataLength);
	if( NULL == _sendCache )
	{
		printf("CommunicationAgent Send Cache create failed!");
		return -1;
	}

	if( 0 != _msgQueue.init(receiveQueueSize) )
	{
		printf("CommunicationAgent message queue init failed!");
		uninit();
		return -2;
	}
	
#ifdef __UDP_TRANSPORT_
	if( RET_OK != _com->addObserver(&_msgQueue,UDP_Protocol) )
	{
		printf("ERR! UDP addObserver \n");
		return -3;
	}
#endif

#ifdef __USB_TRANSPORT_
    if( RET_OK != _com->addObserver(&_msgQueue,USB_Protocol) )
	{
		printf("ERR! USB addObserver \n");
		return -3;
	}
#endif

#ifdef __UART_TRANSPORT_
    if( RET_OK != _com->addObserver(&_msgQueue,UART_Protocol) )
	{
		printf("ERR! UART addObserver \n");
		return -3;
	}
#endif

#ifdef MASTER_SIMULATOR
	MasterMachineSimulator::getInstance()->setMessageQueque( &_msgQueue );
#endif

	_currentMsgProtocol = BROAD_Protocol;
	//printf("initCommunicationAgent \n");
	return 0;
}

int CommunicationAgent::setupUsbProtocolDynamic()
{
#define USB_DEV_PATH   "/dev/transport"
	if(NULL ==_com->getUsbProtocol())
	{
		if(file_util::is_file_read_write_ok(USB_DEV_PATH))
		{
			if(RET_OK == _com->setupUsbProtocol())
			{
				_com->addObserver(&_msgQueue,USB_Protocol);
				return RET_OK;
			}
		}
	}
	return RET_NG;
}

void CommunicationAgent::uninit(void)
{
	//printf("****CommunicationAgent uninit\n");
	Communication::getInstances()->uninit();
	
	if( NULL != _sendCache )
	{
		Message::destroyInstance(&_sendCache);
		_sendCache = NULL;
	}
	if( NULL != _com )
	{
#ifdef __UDP_TRANSPORT_
        _com->removerObverser(&_msgQueue,UDP_Protocol);
#endif

#ifdef __USB_TRANSPORT_
        _com->removerObverser(&_msgQueue,USB_Protocol);
#endif

#ifdef __UART_TRANSPORT_
		_com->removerObverser(&_msgQueue,UART_Protocol);
#endif
	}
	_msgQueue.uninit();

	delete _instance;
	_instance = NULL;
	//printf("_instance = %x \n",_instance);
}



Message* CommunicationAgent::receiveMsg(void)
{
	Message* pMsg = _msgQueue.get();
	return pMsg;
}

void CommunicationAgent::releaseMsg(Message* msg)
{
	if( NULL == msg )
		return;
	_msgQueue.release(msg);
}
/*
unsigned char cmdFlag:下发的flag
short int cmdCode:下发的CommandCode
short int seqNum,：下发的Sequence Number序列号
char* data：下发除了前面12位外后面的数据
int dataLen：数据的总长度Data Length；不包含消息头
SentToType type：发送数据的类型：USB，Uart，SendToOuter等
*/
int CommunicationAgent::sendMsg(unsigned char cmdFlag,short int cmdCode,
								short int seqNum, char* data, int dataLen,
								SentToType protocol)
{
	int nRet = 0;

	if( SendToOuter == protocol )
	{
		_sendCache->reset(cmdFlag,cmdCode,seqNum,data,dataLen);
		
#ifdef __UDP_TRANSPORT_
		nRet = _com->sendMsgPush((unsigned char*)_sendCache->getBuff(), _sendCache->getLength(), UDP_Protocol);
#endif

#ifdef __USB_TRANSPORT_
		nRet = _com->sendMsgPush((unsigned char*)_sendCache->getBuff(), _sendCache->getLength(), USB_Protocol);
#endif
		
#ifdef __UART_TRANSPORT_
		unsigned char * temBuf = (unsigned char *)_sendCache->getBuff();
		

		nRet =  _com->sendMsgPush(temBuf, _sendCache->getMsgSize(temBuf), UART_Protocol);
	//printf("[LX]_com->sendMsgPush  _sendCache->getMsgSize(temBuf) = %d,_sendCache = %x\n",  _sendCache->getMsgSize(temBuf),_sendCache);
#endif

	}
	else if( SendToInner == protocol )
	{
		
	}
	else
	{
		// NOP
	}
	return nRet;
}

int CommunicationAgent::sendReq( Requestion * pReq )
{
	return RequestManage::getInstance()->setRequest( pReq );
}

void CommunicationAgent::addFilter(MessageFilterItf* filter)
{
	_msgQueue.addFilter(filter);
}

void CommunicationAgent::removeFilter(MessageFilterItf* filter)
{
	_msgQueue.removeFilter( filter );
}


