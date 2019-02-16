#pragma once
//opencv
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/video.hpp"
#include "opencv2/videoio.hpp"
//C++
#include <iostream>
#include <sstream>
//own
#include "constants.h"

class Drawer{
public:
	// Draws graph of the percentages and a coordinate system for it.
	void drawStatisticsOnImage(cv::Mat&, double percentages[], int count);
};