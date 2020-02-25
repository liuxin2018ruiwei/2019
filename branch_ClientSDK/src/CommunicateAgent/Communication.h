#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include<pthread.h>
#include "Version.h"
#include"Protocol.h"
#include"BaseThread.h"
//#include "Observer.h"
#include "MessageQueue.h"
#include "DetectFaceIFCommon.h"

#define NO_OPERATE_MAX 10
#define WAIT_TIME_INTERVAL 100

class Observer;
class UartProtocol;
class UDPProtocol;
class USBProtocol;

//class AppContexts;
class Communication:public BaseThread 
{
public:
	// pObver (in) the pointer to add
	// type (in) the protocol type, if specified type, the pObver will save into the specified observers
	virtual int addObserver(Observer* pObver, ProtocolType type);
	// pObver (in) the pointer of observer to add,and then ,when received data,the observer will be notify
	//virtual int addObserver(Observer* pObver);
	// pObj (in) the pointer of the object,make the pthread run corrently
	//static void communicatePthread(void* pObj);
	// get the pointer of communitcation obj;
	static Communication* getInstances();
	// init the communication obj before start to use it
	// inherit from BaseThread
	virtual int init(void* priData);
	int setupUsbProtocol();
	Protocol* getUsbProtocol(){return m_UsbProtocol;};
	// type (in) specified the protocol to receive data
	// return (out) the length have received
	virtual int receiveData(ProtocolType type);
	// pObver (in) the pointer of observer to remove, when the observer obj was destoryed, it must be remove first
	// type (in) the protocol type, if specified type, the pObver will save into the specified observers
	virtual int removerObverser(Observer* pObver, ProtocolType type);
	// pObver (in) the pointer of observer to remove, when the observer obj was destoryed, it must be remove first
	//virtual int removerObverser(Observer* pObver);	
	// type (in) specified the protocol to send data
	// return (out?? the length have send
	virtual int sendData(ProtocolType type);
	// pData (in) the pointer of the data  to send ; nDataLen (in) the length of the data
	//virtual int sendData(unsigned char* pData, int nDataLen, ProtocolType type = ProtocolMax);
	int sendMsgPush(unsigned char* pData, int nDataLen, ProtocolType type);
	// call start to make communication to start to work
	//virtual int start();
	// before closing the work, terminate must be call to free some resource
	// inherit from BaseThread
	virtual void uninit();
	virtual ~Communication();
	int setBautrate(UINT32 braudRateData);
private:
	Communication();
	// inherit from BaseThread
	virtual void run();

	// keep the pointer of itself
	static Communication* m_Communication;
	// specified the observer list to operate( add /remover observer)
	//ProtocolType m_ObversIndx;
	// keep the thread id
	//pthread_t m_PthreadID;
	
	// the pointer of  app context, saved some globl info
	//AppContexts* m_pAppContxt;

	// keep the pointer of the UartProtocol which contains all info about uart
	Protocol* m_UartProtocol;
	// keep the pointer of the uart observer
	//ObsverList m_UartObvers;
	// keep the pointer of the UdpProtocol which contains all info about Udp
	Protocol* m_UdpProtocol;
	// keep the pointer of the udp observer
	//ObsverList m_UdpObvers;
	// keep the pointer of the UsbProtocol which contains all info about usb
	Protocol* m_UsbProtocol;
	// keep the pointer of the usb observer
	//ObsverList m_UsbObvers;

	SendMessageQueue *uartSendQueue;
	SendMessageQueue *udpSendQueue;
	SendMessageQueue *usbSendQueue;
	SendMessageQueue *rdtRs22SendQueue;
};

#endif

