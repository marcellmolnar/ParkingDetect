#pragma once
//opencv
#include <opencv2/shape.hpp>
//C++
#include <ctime>

namespace shapes {

	class Point {
	public:
		int x, y;
		Point(int x = 0, int y = 0) {
			this->x = x;
			this->y = y;
		}
		Point(const Point& other) {
			x = other.x;
			y = other.y;
		}
		operator cv::Point() {
			return cv::Point(x,y);
		}
		int operator[] (int index) {
			if (index < 0 || index > 1)
				throw "Index out of range";
			else {
				if (index == 0)
					return x;
				else
					return y;
			}
		}
	};

	class Rect {
	public:
		Point corners[4];
		double minx, maxx, miny, maxy;
		Rect() {
			init(Point(), Point(), Point(), Point());
		}
		Rect(Point p1, Point p2, Point p3, Point p4) {
			init(p1, p2, p3, p4);
		}
		Rect(const Rect& other) {
			init(other.corners[0], other.corners[1], other.corners[2], other.corners[3]);
		}
		inline void init(Point p1, Point p2, Point p3, Point p4) {
			corners[0] = p1;
			corners[1] = p2;
			corners[2] = p3;
			corners[3] = p4;
			minx = corners[0].x;
			if (corners[1].x < minx)
				minx = corners[1].x;
			maxx = corners[2].x;
			if (corners[3].x > maxx)
				maxx = corners[3].x;
			miny = corners[0].x;
			if (corners[3].x < miny)
				miny = corners[3].x;
			maxy = corners[1].x;
			if (corners[2].x > maxy)
				maxy = corners[2].x;
		}
		Point operator[] (int index) {
			if (index < 0 || index > 3)
				throw "Index out of range";
			else
				return corners[index];
		}
	};
}

class Parking_lot
{
	shapes::Rect rect;
	bool isOccupied;
	bool isOccupiedNew;
	time_t occupiedSince;
	time_t newStatusSince;

public:
	Parking_lot(shapes::Point p1, shapes::Point p2, shapes::Point p3, shapes::Point p4);
	~Parking_lot();
};

