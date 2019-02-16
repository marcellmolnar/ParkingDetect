#pragma once
//opencv
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/video.hpp"
#include "opencv2/videoio.hpp"
//C
#include <stdio.h>
//C++
#include <iostream>
#include <sstream>
//own
#include "Parking_spot.h"

class Utils{
public:
	void filterBlack(const cv::Mat& imageHSV, cv::Mat& imageGrayNew, double grayValue);

	void getDiffImageInGray(const cv::Mat& image1, const cv::Mat& image2, cv::Mat& result, double thresholdVal = 10);

	double meanOfArea(const cv::Mat& imageGray, const cv::Mat& maskAsphalt);

	double meanOfAsphalt();

	// Calulates the number of the non-zero-valued pixels of an area. (or the percentage to the area size)
	double calcNonZeroPixels(const cv::Mat& blackAndWhite, const cv::Mat& mask, bool percentage) const;

	shapes::Rect getRect(double x, double width);

	void draw_rectangle_on_image(cv::Mat& image, shapes::Rect rect, cv::Scalar color, int width);

	void drawOnRectangles(cv::Mat& image, int start, int diff);
};