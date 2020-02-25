#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include "Communication.h"
#include "UartProtocol.h"
#include "UDPProtocol.h"
//#include "USBProtocol.h"
//#include "AppContexts.h"
//#include "log.h"
#include "Observer.h"
#include "ObjectOfObserver.h"
#include "Version.h"

Communication* Communication::m_Communication = NULL;

Communication::Communication():m_UartProtocol(NULL), m_UdpProtocol(NULL),m_UsbProtocol(NULL)
{
}

// pObver (in) the pointer to add
// type (in) the protocol type, if specified type, the pObver will save into the specified observers
int Communication::addObserver(Observer* pObver, ProtocolType type)
{
	if (NULL == pObver)
	{
		printf("Communication::addObserver Params Invalid\n");
		return RET_NG;
	}

	//ObjectOfObserver* pObj = NULL;
	if ((UART_Protocol == type)	&& (m_UartProtocol != NULL) )
	{
		// find the Observer and delete it
		UartProtocol::getInstance()->addObserver(pObver);
		//pObj = dynamic_cast<ObjectOfObserver*>(m_UartProtocol);
		//pObj = (ObjectOfObserver*)m_UartProtocol;
		//pObj = UartProtocol::getInstance();
	}
	else if ((UDP_Protocol == type) && (m_UdpProtocol != NULL) )
	{
		// find the Observer and delete it
		UDPProtocol::getInstance()->addObserver(pObver);
		//pObj = dynamic_cast<ObjectOfObserver*>(m_UdpProtocol);
		//pObj = (ObjectOfObserver*)m_UdpProtocol;
	}
#if 0
    else if ((USB_Protocol == type) && (m_UsbProtocol != NULL) )
	{
		// find the Observer and delete it
		USBProtocol::getInstance()->addObserver(pObver);
		//pObj = dynamic_cast<ObjectOfObserver*>(m_UsbProtocol);
		//pObj = (ObjectOfObserver*)m_UsbProtocol;
	}
#endif
	else
	{
		// no operate
	}

	return RET_OK;
	
}


Communication::~Communication()
{
	if (NULL != m_UartProtocol)
	{
		delete m_UartProtocol;
		m_UartProtocol = NULL;
	}

	if (NULL != m_UdpProtocol)
	{
		delete m_UdpProtocol;
		m_UdpProtocol = NULL;
	}

    if (NULL != m_UsbProtocol)
	{
		delete m_UsbProtocol;
		m_UsbProtocol = NULL;
	}
	m_Communication = NULL;
}

Communication* Communication::getInstances()
{
	if (NULL == m_Communication)
	{
		//printf(" Communication::getInstances new");
		m_Communication = new Communication;
	}
	//printf(" Communication::getInstances get");
	return m_Communication;
}


// init the communication obj before start to use it
// inherit from BaseThread
int Communication::init(void* priData)
{	
	int nRet = -1;
    
#ifdef __USB_TRANSPORT_
	usbSendQueue = new SendMessageQueue();
	usbSendQueue->init(SendMessageQueue::MAX_SEND_QUEUE_SIZE);

	// create the USB communication
	m_UsbProtocol = USBProtocol::getInstance();
	if (NULL == m_UsbProtocol)
	{
		printf("Communication::init  get USB failed");
		return RET_NG;
	}
	// init the USB
	nRet = m_UsbProtocol->init();
	m_UsbProtocol->setSendQueue(usbSendQueue);
	if(nRet != RET_OK)
	{
		m_UsbProtocol->terminate();
		delete m_UsbProtocol;
		m_UsbProtocol = NULL;
	}
#endif

#ifdef __UDP_TRANSPORT_
	udpSendQueue = new SendMessageQueue();
	udpSendQueue->init(SendMessageQueue::MAX_SEND_QUEUE_SIZE);
	
	// create the UDP communication
	m_UdpProtocol = UDPProtocol::getInstance();
	if (NULL == m_UdpProtocol)
	{
		printf("Communication::init  get UDP failed");
		return RET_NG;
	}
	// init the UDP
	nRet = m_UdpProtocol->init();
	m_UdpProtocol->setSendQueue(udpSendQueue);
#endif

#ifdef __UART_TRANSPORT_
	uartSendQueue = new SendMessageQueue();
	nRet = uartSendQueue->init(SendMessageQueue::MAX_SEND_QUEUE_SIZE);
	
	// create the Uart communication
	m_UartProtocol = UartProtocol::getInstance();
	if (NULL == m_UartProtocol)
	{
		printf("Communication::init  get Uart failed");
		return RET_NG;
	}
	// init the Uart
	SerialPortInfo* pPortInfoPtr = (SerialPortInfo*)priData;

	
	nRet = UartProtocol::getInstance()->init(pPortInfoPtr); 
	m_UartProtocol->setSendQueue(uartSendQueue);

#endif

	return nRet;
}

int  Communication::setBautrate(UINT32 braudRateData)
{
	m_UartProtocol = UartProtocol::getInstance();
	if (NULL == m_UartProtocol)
	{
		printf("Communication::init  get Uart failed");
		return RET_NG;
	}
	// init the Uart
	int ret23 = UartProtocol::getInstance()->setBaudrate((int)braudRateData)  ;
	return  ret23; 
}

int Communication::setupUsbProtocol()
{
#if 0// create the USB communication
	Protocol*usbProtocol = USBProtocol::getInstance();
	if (NULL == usbProtocol)
	{
		printf("Communication::init  get USB failed");
		return RET_NG;
	}
	// init the USB
	int nRet = usbProtocol->init();
	usbProtocol->setSendQueue(usbSendQueue);
	if(nRet != RET_OK){
		usbProtocol->terminate();
		delete usbProtocol;
		usbProtocol = NULL;
		return RET_NG;
	}

	m_UsbProtocol = usbProtocol;
#endif
	return RET_OK;
}

// type (in) specified the protocol to receive data
// return (out) the length have received
int Communication::receiveData(ProtocolType type)
{
	//printf("Communication::receiveData ProtocolType %d\n", type);
	int nDataLen = 0;
	if ( (UART_Protocol == type) &&	( NULL != m_UartProtocol) )
	{
		nDataLen = m_UartProtocol->receiveData();
	}
	else if ((UDP_Protocol == type) && (NULL != m_UdpProtocol))
	{
		nDataLen = m_UdpProtocol->receiveData();
	}
	else if ((USB_Protocol == type) && (NULL != m_UsbProtocol))
	{
		nDataLen = m_UsbProtocol->receiveData();
	}
	else
	{
		// no opreate
	}

	return nDataLen;
}


// pObver (in) the pointer of observer to remove, when the observer obj was destoryed, it must be remove first
// type (in) the protocol type, if specified type, the pObver will save into the specified observers
int Communication::removerObverser(Observer* pObver, ProtocolType type)
{
	if (NULL == pObver)
	{
		return RET_NG;
	}

	ObjectOfObserver* pObj = NULL;
	if (UART_Protocol == type)
	{
		// find the Observer and delete it
		//pObj = dynamic_cast<ObjectOfObserver*>(m_UartProtocol);
		pObj = UartProtocol::getInstance();
	}
	else if (UDP_Protocol == type)
	{
		// find the Observer and delete it
		//pObj = dynamic_cast<ObjectOfObserver*>(m_UdpProtocol);
		pObj = UDPProtocol::getInstance();
	}
#if 0
    else if (USB_Protocol == type)
	{
		// find the Observer and delete it
		//pObj = dynamic_cast<ObjectOfObserver*>(m_UsbProtocol);
		pObj = USBProtocol::getInstance();
	}
#endif
	else
	{
		// no operate
	}

	if (NULL != pObj)
	{
		pObj->removerObverser(pObver);
	}

	return RET_OK;
}

// inherit from BaseThread--接收线程的
void Communication::run()
{
	Communication* pCommunicate = this;
	int nFreeCount = 0;
	int nRet = 0;

	//printf("Communication::run  pid=%ld\n", getThreadId());
	while (isRunning())
	{
		nRet = 0;
#ifdef __UART_TRANSPORT_
		nRet = pCommunicate->receiveData(UART_Protocol);
#endif

#ifdef __UDP_TRANSPORT_
		nRet += pCommunicate->receiveData(UDP_Protocol);
#endif

#ifdef __USB_TRANSPORT_
		int readLen = pCommunicate->receiveData(USB_Protocol);
		// if(readLen < 0){
		// 	m_UsbProtocol->terminate();
		// 	delete m_UsbProtocol;
		// 	m_UsbProtocol = NULL;
		// }
		// else{
		// 	nRet += readLen;
		// }
#endif


		
		//ptread_testcancle();
#ifdef __UART_TRANSPORT_
		nRet += pCommunicate->sendData(UART_Protocol);
#endif

#ifdef __UDP_TRANSPORT_
		nRet += pCommunicate->sendData(UDP_Protocol);
#endif

#ifdef __USB_TRANSPORT_
		nRet += pCommunicate->sendData(USB_Protocol);
#endif

		//pthread_testcancel();
		// nRet== 0 means there 's no operate of receive nor send, wait for some time
		// the max time to wait is NO_OPERATE_MAX * WAIT_TIME_INTERVAL
		if (0 == nRet)
		{
			nFreeCount++;
			nFreeCount = (nFreeCount < NO_OPERATE_MAX) ? nFreeCount : NO_OPERATE_MAX;
			usleep(nFreeCount * WAIT_TIME_INTERVAL * 1000);
		}
		else
		{
			nFreeCount = 0;
		}
	}
	//printf("Communication::run  end");

}

// type (in) specified the protocol to send data
// return (out?? the length have send
int Communication::sendData(ProtocolType type)
{
#if 0
	//printf("Communication::sendData  ProtocolType %d", type);
	int nDataLen = 0;
	if ((UART_Protocol == type) && (NULL != m_UartProtocol))
	{
		nDataLen = m_UartProtocol->sendData();
	}
	else if ((UDP_Protocol == type) && (NULL != m_UdpProtocol))
	{
		nDataLen = m_UdpProtocol->sendData();
	}
    	else if ((USB_Protocol == type) && (NULL != m_UsbProtocol))
	{
		nDataLen = m_UsbProtocol->sendData();
	}
	else
	{
		// no opreate
	}
#else
	int nDataLen = 0;
	Message *msg = NULL;
	if ((UART_Protocol == type) && (NULL != m_UartProtocol))
	{
		//nDataLen = m_UartProtocol->sendData();
		msg = uartSendQueue->get();
		if(msg != NULL){
			//printf("write uart len = %d", msg->getLength());
			//std::string str((char *)msg->getBuff(), msg->getLength());
			//nDataLen = m_UartProtocol->sendData((unsigned char*)str.c_str(), str.size());
			
			unsigned char * temBuf = (unsigned char *)msg->getBuff();
			nDataLen = m_UartProtocol->sendData(temBuf, msg->getMsgSize(temBuf) );
			//printf("[LX]sendData  write uart msg->getMsgSize(temBuf) = %d,msg = %x\n", msg->getMsgSize(temBuf),msg);
			uartSendQueue->release(msg);
		}
	}
	else if ((UDP_Protocol == type) && (NULL != m_UdpProtocol))
	{
		msg = udpSendQueue->get();
		if(msg != NULL){
			nDataLen = m_UdpProtocol->sendData((unsigned char *)msg->getBuff(), msg->getLength());
			udpSendQueue->release(msg);
		}
	}
    	else if ((USB_Protocol == type) && (NULL != m_UsbProtocol))
	{
		msg = usbSendQueue->get();
		if(msg != NULL){
			//printf("write usb");
			nDataLen = m_UsbProtocol->sendData((unsigned char *)msg->getBuff(), msg->getLength());
			usbSendQueue->release(msg);
		}
	}
	else
	{
		// no opreate
	}

#endif

	return nDataLen;
}

#if 0
// pData (in) the pointer of the data  to send ; nDataLen (in) the length of the data
int Communication::sendData(unsigned char* pData, int nDataLen, ProtocolType type)
{
	if ( (UART_Protocol == type) && ( NULL != m_UartProtocol) )
	{
		m_UartProtocol->saveData(pData, nDataLen);
	}
	else if ((UDP_Protocol == type) && (NULL != m_UdpProtocol))
	{
		m_UdpProtocol->saveData(pData, nDataLen);
	}
	else if ((USB_Protocol == type) && (NULL != m_UsbProtocol))
	{
		m_UsbProtocol->saveData(pData, nDataLen);
	}
	else
	{
		// no operate
	}
	return 0;
}
#endif

int Communication::sendMsgPush(unsigned char* pData, int nDataLen, ProtocolType type)
{
	int ret32 = 0;
	if ( (UART_Protocol == type) && ( NULL != uartSendQueue) )
	{
		ret32 = uartSendQueue->push(pData, nDataLen);
	}
	else if ((UDP_Protocol == type) && (NULL != udpSendQueue))
	{
		ret32 |= udpSendQueue->push(pData, nDataLen);
	}
	else if ((USB_Protocol == type) && (NULL != usbSendQueue))
	{
		ret32 |= usbSendQueue->push(pData, nDataLen);
	}
	else
	{
		// no operate
	}
	return ret32;
}


// before closing the work, terminate must be call to free some resource
// inherit from BaseThread
void Communication::uninit()
{
	if (true == isRunning())
	{
		//pthread_cancel(getThreadId());
		//pthread_join(getThreadId(), NULL);
		stop();
	}

	if(NULL != uartSendQueue)
	{
		//release data 
		uartSendQueue->uninit();
	}
	
	if (NULL != m_UartProtocol)
	{
		m_UartProtocol->terminate();
	}

	if(NULL != udpSendQueue)
	{
		udpSendQueue->uninit();
	}
	
	if (NULL != m_UdpProtocol)
	{
		m_UdpProtocol->terminate();
	}

	if(NULL != usbSendQueue)
	{
		usbSendQueue->uninit();
	}
	if (NULL != m_UsbProtocol)
	{
		m_UsbProtocol->terminate();
	}
	return;
}
