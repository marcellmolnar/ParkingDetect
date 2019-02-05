#pragma once
//opencv
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "constants.h"
//C++
#include <iostream>
//own
#include "Utils.h"

// Calculate some weird stuff.. (but it works)
void calcPercentages(const cv::Mat& thresh, const cv::Mat& diffImage, cv::Mat* spotMasks, double* percentages, int count);

/* Detect the parking cars from the percentages given.
Every percentage will be averaged with the adjecent 2 percentages */
int* calcParkinglotsStatus(cv::Mat& image, double* percentages, int count, int& numberOfCars);
