#pragma once
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "constants.h"

using namespace cv;

double *percentages = new double[NUMBER_OF_POINTS]();
double *percentagesToDraw = new double[NUMBER_OF_POINTS]();
bool *canRefresh = new bool[NUMBER_OF_POINTS]();

void calcPercentages(const Mat& thresh, const Mat& diffImage);