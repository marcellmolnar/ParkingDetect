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

using namespace cv;
using namespace shapes;

void filterBlack(const Mat& image, Mat& imageGrayNew, double grayValue);

shapes::Rect getRect(double x, double width);

void draw_rectangle_on_image(Mat& image, shapes::Rect rect, Scalar color, uint8_t width);

void drawOnRectangles(Mat& image, uint16_t start, uint16_t diff);
