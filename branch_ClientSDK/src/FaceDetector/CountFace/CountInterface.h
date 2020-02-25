/********************************************************************
* Name      :  CountInterface.h                                  
* Describ   :                                                        
* Author    :  Zhang Botong                                          
* Date      :  2018-04-02                                                 
********************************************************************/

#ifndef COUNTINTERFACE_H
#define COUNTINTERFACE_H

#include <list>
using std::list;
class CountPeople;

class CountInterface
{
 public:


  typedef struct _strPoint
  {
	int X;
	int Y;
  }Point;
  
  typedef struct _strLine
  {
	Point Head;
	Point End;
  }Line;
  
  typedef struct _strCounterInfo
  {
	unsigned int ComeIn;
	unsigned int GetOut;
  }CounterInfo;

  typedef long int TimeSTP;
  typedef struct _strTrackPoint
  {
	unsigned int TrackId;
	Point TrackPoint;
	TimeSTP Timer;
	Point EndPoint;
	TimeSTP EndTimer;
  }TrackPoint;

  typedef list< CountPeople > CounterList;
  typedef CounterList::iterator CountItor;

  typedef list< TrackPoint > PointList;
  typedef PointList::iterator PointItr;

  static const long TimerThresh = 3;
  static const unsigned int ListMax = 120;
  static const int SomeoneComeIn = 1;
  static const int NoTransfer = 0;
  static const int SomeoneGetOut = -1;
  /*
(0,0)--------------------------------->X
    |                            |
    |              get out       |
    |                ^           |
    |     |          |           |
    |     |          |           |
	|_____|__________|____________StandardLine
    |     |          |           |
    |     |          |           |
    |     |          |           |
    |     V                      |
    |  come in                   |
    |             ^  |           |
    |             |  |           |
    |             |  V           |
    |            no transfer     |
	|----------------------------(width, height)
	|
  Y	|
    V
	
   */

  /*
	|<----------DistanceThresh------------->|
	|--------unavailabel shift---->         |
	|------------------availabel shift------------------>
   */
  static const int DistanceThresh = 30 * 30;
  // get the information of the counter
  int getCounterInfo( CounterInfo &info );
  
  static CountInterface *getInstance();
  
  // clear up the info in the counter
  int resetCounterInfo();
  // set a line to judge if the trajectory get though it or not
  // base on the coordinate system of frame( 0,0) to ( widht, height)
  int setStandardLine( Line standardLin );
  int cleanupLine();

  // set a point with trackId, which will be an end point of trajectory
  // it's considered that they are the end points of  trajectory if set tow points with the same track Id
  // return:
  // SomeoneComeIn : someone come in
  // NoTransfer : nobody come in or get out
  // SomeoneGetOut: someone get out
  int setTrackPoint( TrackPoint &point );

  virtual ~CountInterface();
  CounterList &  getCounterList();

  int releaseData();

 protected:
  // return x: the sum of point found
  int refreshEndPoint( TrackPoint &dstPoint, TrackPoint &srcPoint );

  int insertPoint( TrackPoint &point );

  int countVector( Point &startPoint, Point &endPoint );

  CountInterface();

  

 private:
  static CountInterface *m_this;
  CounterInfo m_counterInfo;
  CounterList m_counterList;
  PointList m_list;
};
#endif
