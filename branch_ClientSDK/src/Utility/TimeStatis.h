#ifndef  _TIME_STATIS_H
#define _TIME_STATIS_H

#include <string>

class TimeStatis{
	public:
		TimeStatis(std::string name);
		~TimeStatis();
		void start();
		void tag();
		void end();
		long long average_duration();
		
	private:
		long long m_start;
		long long m_end;
		long long m_total;
		int m_count;
		std::string m_name;
};

#endif

