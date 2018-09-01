#pragma once
//opencv
#include "opencv2/imgcodecs.hpp"

using namespace cv;

const uint16_t H = 720;
const uint16_t W = 1280;

const int START_POINT = 950;
const int END_POINT = 60;
const int STEP_SIZE = 10;
const int NUMBER_OF_POINTS = (START_POINT - END_POINT) / STEP_SIZE;
