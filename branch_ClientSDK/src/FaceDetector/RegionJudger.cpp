/********************************************************************
 * Name      :  RegionJudger.cpp                                  
 * Describ   :  the defination of the class
 * Author    :  Zhang Botong                                          
 * Date      :  2018-06-19                                                 
 ********************************************************************/
#include<string.h>
#include<stdio.h>
#include "RegionJudger.h"

RegionJudger::RegionJudger()
{
	memset( &m_region, 0, sizeof( Region ) );
}

float RegionJudger::computeAreaOfRegion(Region &region )
{
	float sum=0;
	for ( int nCnt = 0; nCnt < region.Sum - 1; nCnt++)
	{  
		sum += ( region.Vertex[nCnt].X * region.Vertex[nCnt + 1].Y
				 - region.Vertex[nCnt + 1].X * region.Vertex[nCnt].Y);  
	}
	
	float area = sum + (region.Vertex[region.Sum - 1].X * region.Vertex[0].Y 
						-  region.Vertex[0].X * region.Vertex[region.Sum - 1].Y );

	if( area < 0.0f )
	{
		area = 0 - area;
	}

	area /= 2;
	//printf("[LX]RegionJudger::computeAreaOfRegion, area=%f\n", area );
	
	return  area;
}

float RegionJudger::getAreaOfRegion()
{
	return m_area;
}

int RegionJudger::getRegion( Region & region )
{
	memcpy( &region, &m_region, sizeof( Region ) );
	return 0;
}

int RegionJudger::setRegion( Region & region )
{
	if( region.Sum < 3 )
	{
		return -1;
	}
	
	memcpy( &m_region, &region, sizeof( Region ) );

	m_area = computeAreaOfRegion( m_region );

	//printf("[LX]RegionJudger::setRegion sum=%d, area=%f\n", m_region.Sum, m_area );
	return 0;
}

bool RegionJudger::isInRegion( Region &region, Point &pPoint )
{
	Region triangle;
	triangle.Vertex[0].X = pPoint.X;
	triangle.Vertex[0].Y = pPoint.Y;
	triangle.Sum = 3;


	float sumArea = 0.0f;
	for( int nCnt = 0; nCnt < m_region.Sum; nCnt++ )
	{
		triangle.Vertex[1].X= m_region.Vertex[nCnt].X;
		triangle.Vertex[1].Y= m_region.Vertex[nCnt].Y;

		triangle.Vertex[2].X= m_region.Vertex[( nCnt+1 ) % m_region.Sum].X;
		triangle.Vertex[2].Y= m_region.Vertex[ (nCnt+1) %  m_region.Sum].Y;

		sumArea += computeAreaOfRegion( triangle );
	}

	float regionArea = getAreaOfRegion();
//	printf("RegionJudger::isInRegion region=%f sum=%f\n", regionArea, sumArea );

	if( ( (regionArea - sumArea ) < 0.000001f )
		&& ( (regionArea - sumArea > -0.000001f ) ) )
	{
		return true;
	}

	
	return false;
}

int RegionJudger:: setRegion(Point *pPoint, int nCount)
{
	if( (pPoint == NULL)||(nCount <= 0)|| ( nCount < VertexMin )|| ( nCount > VertexMax ))
	{
		return -1;
	}
	
	m_region.Sum = nCount;
	Point *pTemp;
	for( int i = 0; (i < nCount)&&(i<VertexMax); i++)
	{
		pTemp = pPoint + i;
		m_region.Vertex[i].X = pTemp->X;
		m_region.Vertex[i].Y = pTemp->Y;
	}
	m_area = computeAreaOfRegion( m_region );
	//printf("[LX] m_area = %f,nCount = %d \n",m_area,nCount);
	return 0;
}



RegionJudger::~RegionJudger()
{
	
}
