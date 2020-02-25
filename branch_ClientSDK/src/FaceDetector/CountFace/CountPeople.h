/********************************************************************
* Name      :  CountPeople.h                                  
* Describ   :  the class is used to count people,it's unsafy to use it in muti-                   pthread
* Author    :  Zhang Botong                                          
* Date      :  2018-04-02                                                 
********************************************************************/

#ifndef COUNTPEOPLE_H
#define COUNTPEOPLE_H

#include "CountInterface.h"

#include <list>

class CountPeople: public CountInterface
{
 public:
  CountPeople();

  // clear up the info in the counter
  //int resetCounterInfo();
  // set a line to judge if the trajectory get though it or not
  // base on the coordinate system of frame( 0,0) to ( widht, height)
  int setStandardLine( Line standardLin );

  int processVector( Point &startPoint, Point &endPoint );


  virtual ~CountPeople();

 protected:
  
  bool isSameSide( Point &point1,Point &point2,
				   Point &headLine,
				   Point &endLine );

  bool shiftLongDistance(Point &point1,Point &point2, int thresh );
 private:
  //  static CountPeople * m_this;
  //CounterInfo m_counterInfo;
  Point m_head;
  Point m_end;
  //PointList m_list;
};
#endif
