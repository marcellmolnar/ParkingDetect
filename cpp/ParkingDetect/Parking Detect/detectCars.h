#pragma once
//opencv
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "constants.h"
//C++
#include <iostream>
//own
#include "Utils.h"

using namespace cv;
using namespace std;

void calcPercentages(const Mat& thresh, const Mat& diffImage, Mat* spotMasks, double* percentages, int count);