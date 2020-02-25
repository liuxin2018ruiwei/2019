/********************************************************************
* Name      :  RegionJudger.h                                  
* Describ   :  the class is used to judge the face is in the region or not
               you can use it to create detect-region or shielded region                                       
* Author    :  Zhang Botong                                          
* Date      :  2018-06-19                                                 
********************************************************************/

#ifndef REGIONJUDGER_H
#define REGIONJUDGER_H

#include "DetectFaceIF.h"

class RegionJudger
{
 public: 

/*  typedef struct _strPoint
  {
	int X;
	int Y;
  }Point;
  */

  enum
  {
  	VertexMin=3,
  	VertexMax=8
  };

  typedef struct _strRegion
  {
	Point Vertex[VertexMax];
	unsigned int Sum;
  }Region;

  RegionJudger();

  virtual int getRegion( Region & region );
  virtual int  setRegion(Point *pPoint, int nCount);

  virtual int setRegion( Region & region );
  
  virtual bool isInRegion( Region &region, Point &pPoint );
  
  
  
  virtual ~RegionJudger();

 protected:
  float getAreaOfRegion();

  float computeAreaOfRegion( Region &region );
  
 private:
  Region m_region;
  float m_area;
};
#endif
