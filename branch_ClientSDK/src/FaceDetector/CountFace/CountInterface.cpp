/********************************************************************
* Name      :  CountInterface.cpp                                  
* Describ   :                                                        
* Author    :  Zhang Botong                                          
* Date      :  2018-04-02                                                 
********************************************************************/

#include <string.h>
#include <sys/time.h>
#include <stdio.h>
#include "CountInterface.h"
#include "CountPeople.h"

CountInterface * CountInterface::m_this =  nullptr;

int CountInterface::countVector( Point &startPoint, Point &endPoint )
{
	int nRet = NoTransfer;
	CountItor itr = m_counterList.begin();
	while( itr != m_counterList.end() )
	{
		nRet = itr->processVector( startPoint, endPoint );
		if( nRet != NoTransfer )
		{
			break;
		}
		
		itr++;
	}

	if( SomeoneGetOut == nRet )
	{
		m_counterInfo.GetOut++;
	}
	else if( SomeoneComeIn == nRet )
	{
		m_counterInfo.ComeIn++;
	}
	else
	{
		// NOP
	}

	return 0;
}

CountInterface *CountInterface::getInstance()
{
	if( nullptr == m_this )
	{
		m_this = new CountInterface;
	}

	return m_this;
}

int CountInterface::getCounterInfo( CounterInfo &info )
{
	struct timeval curTime;
	gettimeofday( &curTime, nullptr );
	unsigned long curTimer = curTime.tv_sec;

	bool bRet = false;
	PointItr itr = m_list.begin();
	while( itr != m_list.end() )
	{
		// printf(" size=%d itr time %ld, cur time=%ld\n",m_list.size(),
		//  	   itr->EndTimer, point.Timer );
		if( ( curTimer - itr->EndTimer) > TimerThresh  )
		{
//			printf(" delete  %ld\n", itr->Timer );
			countVector( itr->TrackPoint, itr->EndPoint );

			itr = m_list.erase( itr );
			//printf(" locate 2\n" );
		}
		else
		{
			itr++;
		}
	}
	
	memcpy( &info,  &m_counterInfo, sizeof( CounterInfo ) );

	return 0;
}

int CountInterface::insertPoint( TrackPoint &point )
{
	memcpy(&point.EndPoint, &point.TrackPoint, sizeof( Point ) );
	point.EndTimer = point.Timer;

	m_list.push_back( point );

	return 0;
}

// clear up the info in the counter
int CountInterface::resetCounterInfo()
{
	memset( &m_counterInfo, 0, sizeof( CounterInfo ) );

	return 0;
}

int CountInterface::releaseData()
{
	m_list.erase(m_list.begin(), m_list.end());
	return 0;
}

int CountInterface::refreshEndPoint( TrackPoint &dstPoint, TrackPoint &srcPoint )
{
	PointItr itr = m_list.begin();
	while( itr != m_list.end() )
	{
		if( srcPoint.TrackId == itr->TrackId )
		{
			// reset the EndPoint with the srcPoint
			memcpy( &(itr->EndPoint), &srcPoint.TrackPoint, sizeof( Point ) );
			itr->EndTimer = srcPoint.Timer;

//			memcpy( &dstPoint, &(*itr), sizeof( TrackPoint ) );

			return 1;
		}
		itr++;
	}

	return 0;
	
}

// set a point with trackId, which will be an end point of trajectory
// it's considered that they are the end points of  trajectory if set tow points with the same track Id
int CountInterface::setTrackPoint( TrackPoint &point )
{
	struct timeval curTime;
	gettimeofday( &curTime, nullptr );
	point.Timer = curTime.tv_sec;

	TrackPoint startPoint;
	int nRet = refreshEndPoint( startPoint, point );
	if( 0 == nRet )
	{
		// insert the point
		insertPoint( point );
		//printf("insert point \n");
	}

	if( m_list.size() < ListMax )
	{
		return m_list.size();
	}

	PointItr itr = m_list.begin();
	while( itr != m_list.end() )
	{
		//printf(" size=%d itr time %ld, cur time=%ld\n",m_list.size(),
		//  	   itr->EndTimer, point.Timer );
		if( (itr->Timer == itr->EndTimer )
			&& ( ( point.Timer - itr->Timer) > TimerThresh * 10 )  )
		{
			//printf(" setTrackPoint size=%d itr time %ld, cur time=%ld\n",m_list.size(),
			//	   itr->EndTimer, point.Timer );
			itr = m_list.erase( itr );
		}
		else
		{
			itr++;
		}
	}

	return m_list.size();
}

// set a line to judge if the trajectory get though it or not
// base on the coordinate system of frame( 0,0) to ( widht, height)
int CountInterface::setStandardLine( Line standardLine )
{
	CountPeople counter;
	m_counterList.push_back( counter );
	m_counterList.back().setStandardLine( standardLine );
}

int CountInterface::cleanupLine()
{
	m_counterList.erase( m_counterList.begin(), m_counterList.end() );
	return 0;
}


list< CountPeople > & CountInterface::getCounterList()
{
	return m_counterList;
}


CountInterface::CountInterface( )
{
}

CountInterface::~CountInterface()
{
}
