#pragma once
//opencv
#include "opencv2/imgcodecs.hpp"

using namespace cv;

const uint16_t H = 720;
const uint16_t W = 1280;

const int START_POINT = 950;
const int END_POINT = 60;
const int STEP_SIZE = 10;
// if STEP_SIZE is divider of (START_POINT - END_POINT) then the division's result will less than the actual quotient
//   but we correct it.
// if STEP_SIZE is NOT divider of (START_POINT - END_POINT) then the division's result won't change, but we round it up
const int NUMBER_OF_POINTS = 1 + (START_POINT - END_POINT - 1) / STEP_SIZE;


const int graphMinY = 500;
const int graphMaxY = 200;

// BGR colors
const Scalar COLOR_BLACK = Scalar(0, 0, 0);
const Scalar COLOR_WHITE = Scalar(255, 255, 255);
const Scalar COLOR_BLUE = Scalar(255, 0, 0);
const Scalar COLOR_RED = Scalar(0, 0, 255);
const Scalar COLOR_GREEN = Scalar(0, 255, 0);
const Scalar COLOR_YELLOW = Scalar(0, 255, 255);

const int circleRadius = 4;
const int circleThickness = 4;