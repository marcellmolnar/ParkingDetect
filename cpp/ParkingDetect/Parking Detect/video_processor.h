#pragma once
//opencv
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
//own
#include "AsphaltHandler.h"
#include "carHandler.h"
#include "constants.h"
#include "detectCars.h"
#include "draw.h"

void processVideo(char* videoFilename, int frameNumber);