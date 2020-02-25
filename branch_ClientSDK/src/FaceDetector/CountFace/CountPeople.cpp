/********************************************************************
* Name      :  CountPeople.cpp                                  
* Describ   :  the defination of the class
* Author    :  Zhang Botong                                          
* Date      :  2018-04-02                                                 
********************************************************************/

#include <string.h>
#include <sys/time.h>
#include "CountPeople.h"

#include <stdio.h>


int CountPeople::processVector( Point &startPoint, Point &endPoint )
{
	bool bRet = isSameSide( startPoint, endPoint, m_head, m_end );

	if( true == bRet )
	{
		return NoTransfer;
	}

	bRet = shiftLongDistance( startPoint, endPoint, DistanceThresh );
	if( false == bRet )
	{
		return NoTransfer;
	}
	
	if( endPoint.Y < startPoint.Y )
	{
//		m_counterInfo.ComeIn++;
		return SomeoneGetOut;
	}
	else
	{
//		m_counterInfo.GetOut++;
		return SomeoneComeIn;
	}
}

bool CountPeople::shiftLongDistance(Point &point1,Point &point2, int thresh )
{
	bool bRet = false;

	int oX = point1.X - point2.X;
	int oY = point1.Y - point2.Y;
	
	if( (oX * oX + oY * oY ) > thresh )
	{
		bRet = true;
	}

	return bRet;
}


// set a line to judge if the trajectory get though it or not
// base on the coordinate system of frame( 0,0) to ( widht, height)
int CountPeople::setStandardLine( Line standardLin )
{
	memcpy( &m_head, &standardLin.Head, sizeof( Point ) );
	memcpy( &m_end, &standardLin.End, sizeof( Point ) );

	return 0;
}


CountPeople::~CountPeople()
{
	
}

CountPeople::CountPeople()
{
	
}


bool CountPeople::isSameSide( Point &point1, Point &point2,
							 Point &headLine, Point &endLine )
{
	// line :  Ax + By + C = 0;
	int nA = endLine.Y - headLine.Y;
	int nB = headLine.X - endLine.X;
	int nC = endLine.X * headLine.Y - headLine.X * endLine.Y;

//	printf("A=%d, B=%d, C=%d\n", nA, nB, nC );
	float nResPoint1 = nA * point1.X + nB * point1.Y + nC;
	float nResPoint2 = nA * point2.X + nB * point2.Y + nC;

	nResPoint1 *= 0.000001f;
	nResPoint2 *= 0.000001f;
	// res : (A*x1 + B*y1 + C) *( A*x2 + B*y2 + C )
	float nRes = nResPoint1 * nResPoint2;
	// printf("Same=%f, (%d, %d), (%d, %d )\n",
	//  	   nRes, point1.X, point1.Y, point2.X, point2.Y );
	if( nRes > 0 )
	{
		return true;
	}
	
	return false;
}

