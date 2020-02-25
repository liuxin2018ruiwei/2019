#ifndef MESSAGE_QUEUE_H__
#define MESSAGE_QUEUE_H__

#include <queue>
//#include <vector>
#include <list>
#include "NonCopyable.h"
#include "RecoLock.h"
#include "Observer.h"
#include "Protocol.h"

class Message : public NonCopyable
{
public:
//HEAD_LENGHT 是指报文从开始到结尾出现正文，不包括COMMAND FLAG SEQ_NUM 4个字节
  static const int HEAD_LENGHT = 12;//14;
  static const int HEAD_LEN_FB_TO_COMMAND = 8;//COMMAND 开始的位置
	//通过工厂方式来避免内部内存申请异常处理
	static Message* createInstance(int dataLen);
	static void destroyInstance(Message** mes);
	
	virtual unsigned int getLength(void) const;	//消息长度
	virtual char getType(void) const;//flags

	//我这边要用4个：commandCode/flags/sequenceNumber/Data
	virtual short int getCode(void) const;//commandCode
	virtual char getFlags(void) const;//flags
	virtual char getSequenceNumber(void) const;//Sequence Number
	virtual char* getData(void);//正文数据位置
	
	virtual short int getReturn(void) const;
	virtual char* getBuff(void);
	int getBuffSize(){return _buffSize;};
	int resize(int dataLen);

	virtual ProtocolType getFromProtocol();
	virtual void setFromProtocol(ProtocolType protocol);
	
	virtual int reset(char cmdFlag,short int cmdCode, short int seqNum, char* data, int dataLen);
	virtual int reset(unsigned char* buff);
	unsigned int getMsgSize(unsigned char* buff) const;
	
	virtual ~Message();
protected:
	Message();
	unsigned short ipCkSum( unsigned char *data, unsigned short len );
	//~Message();
private:
	static const int MAGIC_NUM_1 = 0xfb;
	static const int MAGIC_NUM_2 = 0x71;
	static const int CHECKSUM_LENGHT = 1;
	static const int RESERVE_LENGTH = 4;
	
	static const int LENGHT_POS = 4;//1;
	static const int RESERVE_POS = 5;
	static const int CHECKSUM_POS = 6;
	static const int CODE_POS = 8; //comamndCode
	static const int TYPE_POS = 10;//flags
	static const int SEQ_NUM_POS = 11;//Sequence Number
	static const int RETURN_POS = 12;//原有协议的返回码位置
	

	//如果长度大于内部buff长度，内部会重新分配BUFF空间，此时消息头会被重置
	//如果需要设置消息头，必须在这个接口之后调用
	//int resetData(unsigned char* data,int len);
	
	//重新计算当前校验和
	void checksum(void);
	
    void setMagicNum(void);
    void setReserve(void);
    void setLength(unsigned int length);
	void setType(char type);
	void setCode(short int code);
	void setReturn(short int ret);
	
	int init(int dataLen);
	void uninit();
	int testAndReinit(int dataLen);

	//通过数据长度计算消息长度
	unsigned int getMsgSize(unsigned int dataSize) const;
	//通过消息长度计算数据长度
	unsigned int getDataSize(unsigned int msgSize) const;

	unsigned int 	_buffSize;	//buff长度，不是消息长度?
	char* 	_buff;
	ProtocolType _fromProtocol;
};


//消息过滤接口,过滤消息需要实现该接口
class MessageFilterItf
{
public:
	/***************************************************
	//MessageQueue的update被调用时，会使用该接口过滤接收到的消息
	//如果消息被过滤(丢弃)则返回true，返回false消息才会进入队列中
	没有被过滤的消息才会被放到消息队列中
	********************************************************/
	virtual	bool filter(Message* mes) = 0;
	virtual bool isExistCmdType( unsigned int cmdType,unsigned int cmdCode);
};

//类型定义提到外面，SendMessageQueue也可以用此定义
typedef std::list< Message* >	MessageList;
typedef MessageList::iterator MessageListItor;
//消息对列，队列中的消息被循环使用，减少资源重复分配带来碎片
class MessageQueue : public Observer
{
public:
  typedef std::list< MessageFilterItf* > MsgFilterList;
  typedef MsgFilterList::iterator MsgFilterItor;
  
  virtual int update(void* pData);

  //初始化，初始化消息
  int init(int queueSize);


	//添加一个消息过滤器，如果有多个过滤器被添加，则按添加的顺序依次被调用
	void addFilter(MessageFilterItf* filter);

	void removeFilter(MessageFilterItf* filter);

	//
	virtual void uninit();
	
	//获取的消息必须调用release放回,否则会内存泄漏
	//成功返回消息指针，失败返回NULL
	Message* get();
	
	//
	void release(Message* mes);
private:
	//默认的消息数据长度
	static const int INIT_DATA_SIZE = 128;

	//virtual bool filter(Message* mes) = 0;

	void clearQueue(std::queue<Message*>& q);
	void clearQueue( MessageList &q);
	
	//std::queue<Message*>	_free;
	//std::queue<Message*>	_used;
	//	std::vector<MessageFilterItf*>	_filters;
	MessageList	_free;
	MessageList	_used;
	MsgFilterList _filters;
	RecoLock	_lock;
	int _size;
};


class SendMessageQueue
{
public:
	SendMessageQueue();
	
	int init(int queueSize);

	virtual int uninit();

	int push(unsigned char * data, int len);

	Message *get();

	void release(Message *msg);

	static const int MAX_SEND_QUEUE_SIZE = 3;

private:
	void clearQueue( MessageList &q);
	
	static const int INIT_DATA_SIZE = 128;
	MessageList _free;
	MessageList _used;
	RecoLock _lock;
	int _size;
};

#endif
