#include <stdio.h>
#include <string.h>
#include "MessageQueue.h"
//#include "log.h"
#include "LittleEndianIO.h"
#include "Version.h"
#include "GetFaceDetecteVersionUCmd.h"
#include "QueryInfoUCmd.h"
#include "PthreadManage.h"



Message* Message::createInstance(int dataLen)
{
	Message* mes = new Message();
	if( NULL == mes )
		return NULL;
	if( 0 != mes->init(dataLen) )
	{
	    printf("Message init failed =%d",dataLen);
		delete mes;
		return NULL;
	}
	return mes;
}

void Message::destroyInstance(Message** mes)
{
	if( NULL == mes )
		return ;
	if( NULL == *mes )
		return ;
	(*mes)->uninit();
	delete *mes;
	*mes = NULL;
}
/*
Message::Message(int dataLen)
	: _data(NULL)
{
	
}
*/
Message::~Message()
{
	uninit();
}

int Message::init(int dataLen)
{
	if( NULL != _buff)
		uninit();
	_buffSize = getMsgSize(dataLen);
	_buff = new char[_buffSize];
	if( NULL == _buff)
		return -1;
	return 0;
}

void Message::uninit()
{
	if( NULL != _buff)
	{
		delete[] _buff;
		_buff = NULL;
	}
	_buffSize = 0;
}


int Message::resize(int dataLen)
{
	uninit();
	if( 0 != init(dataLen) )
		return -1;

	return 0;
}


int Message::testAndReinit(int dataLen)
{
	unsigned int newMsgSize = getMsgSize(dataLen);
	//printf("[LX] testAndReinit newMsgSize = %d,_buffSize = %d\n",newMsgSize,_buffSize);
	if( _buffSize < newMsgSize )
	{
		uninit();
		if( 0 != init(dataLen) )
			return -1;
	}
	return 0;
}



Message::Message()
		: 	_buff(NULL),
			_buffSize(0)
{

}


unsigned int Message::getLength(void) const
{
	
//	return LittleEndianIO::readInt32(_buff+LENGHT_POS);
	return LittleEndianIO::readInt16(_buff+LENGHT_POS);
}

void Message::setLength(unsigned int length)
{
	//_msgSize = length;
//	LittleEndianIO::writeInt32( _buff+LENGHT_POS, length);
	LittleEndianIO::writeInt16( _buff+LENGHT_POS, length);
}



char Message::getType(void) const
{
	return _buff[TYPE_POS];
}

char Message::getFlags(void) const
{
	return getType();
}

char Message::getSequenceNumber(void) const
{
	return _buff[SEQ_NUM_POS];
}



void Message::setType(char type)
{
	_buff[TYPE_POS] = type;
}


short int Message::getCode(void) const
{
	return LittleEndianIO::readInt16(_buff+CODE_POS);
}

void Message::setCode(short int code)
{
	LittleEndianIO::writeInt16( _buff+CODE_POS, code);
}


short int Message::getReturn(void) const
{
	return LittleEndianIO::readInt16(_buff+RETURN_POS);
}

void Message::setReturn(short int ret)
{
	LittleEndianIO::writeInt16( _buff+RETURN_POS, ret);
}

void Message::setMagicNum(void)
{
	_buff[0] = (char)MAGIC_NUM_1;
	_buff[1] = (char)MAGIC_NUM_2;
}

void Message::setReserve()
{
    for(int i = RESERVE_POS; i < RESERVE_POS+RESERVE_LENGTH; i++)
    {
	    _buff[i] = 0x00;
    }
}

void Message::checksum()
{
	unsigned short sum = 0x00;
	unsigned char * tempBuf = (unsigned char *)getBuff();
	unsigned int msgSize = getMsgSize(tempBuf);//getLength();
	//printf("[LX] checksum  msgSize = %d， this = %x \n",  msgSize, this);

	_buff[CHECKSUM_POS] = 0;
	_buff[CHECKSUM_POS + 1] = 0;
	
	*( (unsigned short*)(_buff + CHECKSUM_POS) ) = ipCkSum( (unsigned char*)_buff,
															msgSize );
}


char* Message::getData(void) 
{
	//printf("_buff = %x",_buff);
	return _buff+HEAD_LENGHT;
}

char* Message::getBuff(void)
{
	return _buff;
}


ProtocolType Message::getFromProtocol()
{
	return _fromProtocol;
}

void Message::setFromProtocol(ProtocolType protocol)
{
	_fromProtocol = protocol;
}

/*
 * Internet Protocol的校验和计算。
 * 16位的补码和和先以16位求和再被码是一致的，这样效率更高一些。不过书上好像
 * 没讲清楚最后计算出来的和如果是超出16位的，则要把高低16位再相加。
 * 
 * 这里计算出来的校验和没必要再字节转换了，直接是网络字节序的。
 */
unsigned short Message::ipCkSum(unsigned char *data, unsigned short len)
{
    unsigned int sum = 0;
    unsigned short nwords = len >> 1;
    unsigned short *word = (unsigned short *)data;
    unsigned short i = 0;

    for (i = 0; i < nwords; ++i)
    {
        sum += word[i];
    }

    // 这里要注意，网上多数是错的，对于小端序的CPU，如果直接用word[i]会导致
    // 后面无用的数据在高字节上，有用的数据反而被0xff00给清零了。
    if (len & 1)
    {
        sum += ((unsigned short)data[i * 2] << 16) & 0xff00;
    }

    // 这一步书上好像没讲。
    sum = (sum >> 16) + (sum & 0xffff);
    sum = ~sum & 0xffff;

    return (unsigned short)sum;
}
/*
char cmdFlag:下发的flag
short int cmdCode:下发的CommandCode
short int seqNum,：下发的Sequence Number序列号
char* data：下发除了前面12位外后面的数据
int dataLen：数据的总长度Data Length；不包含消息头
*/
int Message::reset(char cmdFlag,short int cmdCode, short int seqNum, char* data, int dataLen)
{
	//resetData(data,dataLen);

	if (0 != testAndReinit(dataLen) )
	{
		return -1;
	}
    int size = getMsgSize(dataLen);
#if 0
    printf("reset size = %d",getMsgSize(dataLen));
    for(int i = 0; i < size; i++)
    {
        printf("%02x ",_buff[i]);
    }
    printf("\n/n ");
#endif
    setMagicNum();
	setLength(getMsgSize(dataLen) - HEAD_LEN_FB_TO_COMMAND);
	// setReserve();
	setType(cmdFlag);
	setCode(cmdCode);
	//setReturn(ret);
	if( NULL != data && dataLen > 0 )
	{
		memcpy(getData(),data,dataLen);
	}
	checksum();
#if 0 // DEBUG
	printf("reset size = %d  ######################\n",getMsgSize(dataLen));
    for(int i = 0; i < size; i++)
    {
        printf("%02x ",*(unsigned char*)(_buff + i));
    }
    printf("\n ");
#endif
    return 0;
}

	


int Message::reset(unsigned char* buff)
{
	//unsigned int curLen = getLength();
	if( NULL == buff )
		return 0;

	//printf("read buf len = %d\n ",LittleEndianIO::readInt16((const char*)buff+LENGHT_POS));
	
	unsigned int newMsgSize = getMsgSize((unsigned char* )buff);//LittleEndianIO::readInt16((const char*)buff+LENGHT_POS) + 8;

	unsigned int dataSize = getDataSize(newMsgSize);
	if (0 != testAndReinit(dataSize) )
	{
		return -1;
	}
	//_msgSize = newMsgSize;
	memcpy(_buff,buff,newMsgSize);
	return 0;
}
/*
int Message::resetData(unsigned char* data,int dataLen)
{
	//unsigned int curLen = getLength();
	//unsigned int len = readInt(buff+LENGHT_POS);
	unsigned int newBuffSize = getBuffSize(dataLen);
	if( _buffSize < newBuffSize )
	{
		uninit();
		if( 0 != init(dataLen) )
			return -1;
	}
	_buffSize = newBuffSize;
	memcpy(&_buff[HEAD_LENGHT],data,dataLen);
	return 0;
}
*/
//通过BUFF长度计算数据长度
//dataSize 指的是除SEQ_NUM后面开始的位置
//HEAD_LENGHT是从头到SEQ_NUM长度是12
unsigned int Message::getMsgSize(unsigned int dataSize) const
{
	//printf("_____readInt16,dataSize = %d\n",dataSize);
	return dataSize + HEAD_LENGHT;// dataSize + (HEAD)8+ (COMMAND)4
}

unsigned int Message::getMsgSize(unsigned char* buff) const
{
	return (LittleEndianIO::readInt16((const char*)buff+LENGHT_POS) + HEAD_LEN_FB_TO_COMMAND);
}



unsigned int Message::getDataSize(unsigned int msgSize) const
{
	return msgSize-HEAD_LENGHT;//-CHECKSUM_LENGHT;
}



bool MessageFilterItf::isExistCmdType( unsigned int cmdType,unsigned int cmdCode)
{
	return true;
}

void MessageQueue::addFilter(MessageFilterItf* filter)
{
	_filters.push_back(filter);
	//printf("_filters.size() = %d\n",_filters.size());
}

void MessageQueue::removeFilter(MessageFilterItf* filter)
{
	RecoLockGuard autolock(_lock);
	MsgFilterItor itr = _filters.begin();
	while( itr != _filters.end() )
	{
		if( filter == *itr )
		{
			_filters.erase(itr );
			break;
		}
		itr++;
	}

}


int MessageQueue::update(void* pData)
{
	//printf(" MessageQueue updata  start\n");
	
	int ret = Observer::UPDATE_NG;
	{
		RecoLockGuard autolock(_lock);
		if( _free.empty() )
		{
			//printf("_free.empty() \n");
			ret = Observer::UPDATE_BUSY; //队列已满
		}
		else
		{
			//printf("update ->reset start\n");
			ProtocolMsg * protoMsg = (ProtocolMsg *)pData;
			Message* mes = _free.front();
			mes->reset(protoMsg->msg_data);
			mes->setFromProtocol(protoMsg->msg_protocol);
			//printf("msg proto = %d\n", protoMsg->msg_protocol);

			//for (std::vector<MessageFilterItf*>::iterator itr = _filters.begin();
			MsgFilterItor itr = _filters.begin();
			int filterRes = Observer::UPDATE_NG;
			for( ; itr != _filters.end(); itr++)
			{
				if( false == (*itr)->filter( mes ) )
				{
					filterRes = Observer::UPDATE_OK;
					//printf(" Observer::filterRes = %d \n",filterRes);
					break;
				}
				/*
				if( (*itr)->filter(mes) )//检查命令是否要处理
				{
					return Observer::UPDATE_NG;//命令被过滤
				}
				*/
			}
			if( Observer::UPDATE_OK == filterRes )
			{
				_free.pop_front();//pop();
				_used.push_back( mes );//push(mes);
				ret = Observer::UPDATE_OK;
				//printf("_used add mes = %x \n",mes);
			
			}
		}
	}
	
	return ret;//Observer::UPDATE_OK;
}

//初始化，初始化消息
int MessageQueue::init(int queueSize)
{
	RecoLockGuard autolock(_lock);
	for( int i = 0; i < queueSize; i++)
	{
		Message* mes = Message::createInstance(INIT_DATA_SIZE);
		if( NULL == mes )
		{
			uninit();
			return -1;
		}
		_free.push_back(mes);// push(mes);
	}
	_size = queueSize;
    return 0;
}

//
void MessageQueue::uninit()
{
	RecoLockGuard autolock(_lock);

	clearQueue(_free);
	clearQueue(_used);

}

void MessageQueue::clearQueue(std::queue<Message*>& q)
{
	while( !q.empty() )
	{
		Message* mes = q.front();
		q.pop();
		if( NULL != mes )
		{
			Message::destroyInstance(&mes);
		}
	}
}

void MessageQueue::clearQueue( MessageList & q)
{
	while( !q.empty() )
	{
		Message* mes = q.front();
		q.pop_front();
		if( NULL != mes )
		{
			Message::destroyInstance(&mes);
		}
	}
}


Message* MessageQueue::get()
{
	Message* mes = NULL;

	RecoLockGuard autolock(_lock);
    if( !_used.empty() )
    {
	    mes = _used.front();
		// delete it to avoid memory-leakage, the memory pointer
		// will be pop just when move from used container to free container
		// _used.pop();
	}
	return mes;
}

//
void MessageQueue::release(Message* mes)
{
	RecoLockGuard autolock(_lock);

	MessageListItor it = _used.begin();
	for( ; it != _used.end(); it++ )
	{
		if( mes == *it )
		{
			_used.erase( it );
			break;
		}
	}
	_free.push_back( mes );//push(mes);
}




SendMessageQueue::SendMessageQueue()
					:_size(0)
{

}
	
int SendMessageQueue::init(int queueSize)
{
	RecoLockGuard autolock(_lock);
	for( int i = 0; i < queueSize; i++)
	{
		Message* msg = Message::createInstance(INIT_DATA_SIZE);
		if( NULL == msg )
		{
			uninit();
			return -1;
		}
		_free.push_back(msg);// push(mes);
	}
	_size = queueSize;
	return 0;
}

int SendMessageQueue::uninit()
{
	RecoLockGuard autolock(_lock);
	clearQueue(_free);
	clearQueue(_used);
	return 0;
}

int SendMessageQueue::push(unsigned char * data, int len)
{
	int ret = Observer::UPDATE_NG;
	RecoLockGuard autolock(_lock);
	if(_free.empty())
	{
		//LOGD("send queue full");
		ret = Observer::UPDATE_BUSY; //队列已满
	}
	else
	{
		Message* msg = _free.front();
		//printf("\n\nSendMessageQueue::push -> reset data = %x\n",data);

		msg->reset((unsigned char *)data);// have memcpy
	
		_free.pop_front();//pop();
		_used.push_back( msg );//push(msg);
		ret = Observer::UPDATE_OK;
	}
	
	return ret;
}

Message *SendMessageQueue::get()
{
	Message* msg = NULL;
	RecoLockGuard autolock(_lock);
	if( !_used.empty() )
	{
		msg = _used.front();
	}
	return msg;
}

void SendMessageQueue::release(Message *msg)
{
	RecoLockGuard autolock(_lock);

	MessageListItor it = _used.begin();
	for( ; it != _used.end(); it++ )
	{
		if( msg == *it )
		{
			_used.erase( it );
			break;
		}
	}

	if(msg->getBuffSize() > (INIT_DATA_SIZE + 15)){
		msg->resize(INIT_DATA_SIZE);
	}
		
	_free.push_back(msg);//push(msg);
}

void SendMessageQueue::clearQueue( MessageList &q)
{
	while( !q.empty() )
	{
		Message* msg = q.front();
		q.pop_front();
		if( NULL != msg )
		{
			Message::destroyInstance(&msg);
		}
	}
}


