#pragma once
//opencv
#include "opencv2/imgcodecs.hpp"

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
const cv::Scalar COLOR_BLACK = cv::Scalar(0, 0, 0);
const cv::Scalar COLOR_WHITE = cv::Scalar(255, 255, 255);
const cv::Scalar COLOR_BLUE = cv::Scalar(255, 0, 0);
const cv::Scalar COLOR_RED = cv::Scalar(0, 0, 255);
const cv::Scalar COLOR_GREEN = cv::Scalar(0, 255, 0);
const cv::Scalar COLOR_YELLOW = cv::Scalar(0, 255, 255);

const int circleRadius = 4;
const int circleThickness = 4;