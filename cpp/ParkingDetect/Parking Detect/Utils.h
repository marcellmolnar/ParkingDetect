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

void filterBlack(const Mat&, Mat&, double);

double meanOfArea(const Mat&, const Mat&, double&, double& );

shapes::Rect getRect(double, double);

void draw_rectangle_on_image(Mat&, shapes::Rect, Scalar, int);

void drawOnRectangles(Mat&, int, int);
