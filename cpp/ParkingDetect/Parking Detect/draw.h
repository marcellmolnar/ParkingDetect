#pragma once
// opencv
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
//C++
#include <iostream>
#include <sstream>
//own
#include "constants.h"

using namespace cv;
using namespace std;


void drawStatisticsOnImage(Mat&, double percentages[], int count);