#include "TimeStatis.h"
#include "file_util.h"
#include "log.h"



TimeStatis::TimeStatis(std::string name)
	:m_start(0), 
	m_end(0),
	m_total(0),
	m_count(0),
	m_name(name)
{

}

TimeStatis::~TimeStatis()
{

}

void TimeStatis::start()
{
	m_start = file_util::getMilliSecond();
}

void TimeStatis::tag()
{
	LOGD("%s  tag = %d ms\n",  m_name.c_str(), (int)(file_util::getMilliSecond() - m_start));
}

void TimeStatis::end()
{
	m_end = file_util::getMilliSecond();
	m_total += (m_end - m_start);
	m_count++;
	LOGD("%s  duration = %d ms\n\n",  m_name.c_str(), (int)(m_end - m_start));

	if( m_count > 50 )
	{
		m_total = 0;
		m_count = 0;
	}
}

long long TimeStatis::average_duration()
{
	if(m_count > 0 ){
		return (m_total/m_count);
	}
	return 0;
}

