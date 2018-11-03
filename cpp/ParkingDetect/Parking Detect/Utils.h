#pragma once
// opencv
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
//C
#include <stdio.h>
//C++
#include <iostream>
#include <sstream>
//own
#include "Parking_lot.h"

class Utils{
public:
	void filterBlack(const cv::Mat&, cv::Mat&, double);

	void getDiffImageInGray(const cv::Mat&, const cv::Mat&, cv::Mat&, double = 10);

	double meanOfArea(const cv::Mat&, const cv::Mat&);

	double meanOfAsphalt();

	// Calulates the number of the non-zero-valued pixels of an area. (or the percentage to the area size)
	double calcNonZeroPixels(const cv::Mat&, const cv::Mat&, bool) const;

	shapes::Rect getRect(double, double);

	void draw_rectangle_on_image(cv::Mat&, shapes::Rect, cv::Scalar, int);

	void drawOnRectangles(cv::Mat&, int, int);
};