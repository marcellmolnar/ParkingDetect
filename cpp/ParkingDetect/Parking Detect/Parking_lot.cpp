#include <iostream>
#include "Parking_lot.h"

using namespace shapes;

Parking_lot::Parking_lot(Point p1, Point p2, Point p3, Point p4)
{
	rect = Rect(p1, p2, p3, p4);
	isOccupied = true;
	isOccupiedNew = false;
	occupiedSince = time(0);
	newStatusSince = time(0);

}


Parking_lot::~Parking_lot()
{
}
