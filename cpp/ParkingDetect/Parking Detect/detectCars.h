#pragma once
//opencv
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
//C++
#include <iostream>
//own
#include "constants.h"
#include "Utils.h"

class DetectCars{
public:
	// Calculate some weird stuff.. (but it works)
	void calcPercentages(const Utils& utils, const cv::Mat& thresh, const cv::Mat& diffImage, cv::Mat* spotMasks, double* percentages, int count);

	/* Detect the parking cars from the percentages given.
	Every percentage will be averaged with the adjecent 2 percentages */
	int* calcParkinglotsStatus(cv::Mat& image, double* percentages, int count, int& numberOfCars);
};