#include "stdafx.h"
#include "Point.h"


Point::Point() {}

Point::~Point() {}

Point::Point(int t_x, int t_y) :x(t_x), y(t_y) {}

void Point::setCordinates(int t_x, int t_y) 
{
	x = t_x;
	y = t_y;
}
