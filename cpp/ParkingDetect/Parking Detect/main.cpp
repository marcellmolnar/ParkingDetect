//opencv
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
#include <thread>
//own
#include "constants.h"
#include "detectCars.h"
#include "draw.h"
#include "Utils.h"

//TBB
#include "tbb/tbb.h"

using namespace cv;
using namespace std;

bool logTimes = false;

#define LOG_TIMES false



// Only for background subtraction.
/*Mat background;
Mat background_gray;*/

void processVideo(char*, int);
bool handleKeyPress(char, bool&);
double fps();

class AsphaltHandler {
	int count = 0;
	double avg = 0;
public:
	/* Color of asphalt tracker. Only update when there are no movement, and we take the average of numberOfValues values. */
	double updateAsphaltColor(double value, double numberOfWhitePixels, double* grayValues, int numberOfValues) {
		if (numberOfWhitePixels < 30) {
			if (count > 0) {
				avg *= count;
				avg -= grayValues[0];
			}
			if (count < numberOfValues)
				count++;
			for (int i = 0; i < numberOfValues - 1; i++) {
				grayValues[i] = grayValues[i + 1];
			}
			grayValues[numberOfValues - 1] = value;

			avg += value;
			avg /= count;
		}
		return avg;
	}
};

int main(int argc, char* argv[])
{
	std::cout << "app started" << std::endl;

	Parking_lot p(shapes::Point(522, 272), shapes::Point(560, 299), shapes::Point(835, 307), shapes::Point(835, 280));
		
	if (argc != 2) {
		cerr << "Incorret input list!" << endl;
		cerr << "exiting..." << endl;
		return EXIT_FAILURE;
	}

	// Only for background subtraction.
	/*background = imread("bg.jpg", IMREAD_COLOR); // Load an image
	if (background.empty())
	{
		cerr << "No background image found!" << endl;
		cerr << "exiting..." << endl;
		return EXIT_FAILURE;
	}
	std::cout << "bckgnd opened" << std::endl;
	cvtColor(background, background_gray, COLOR_BGR2GRAY);*/

	// create window
	namedWindow("Frame0");
	std::cout << "window(s) created" << std::endl;

	// Name and full path of the video.
	char file[52+2+4+2];
	strcpy_s(file, "C:/Users/Marci/Desktop/Smart City/Park Detect/vids/");
	strcat_s(file, argv[1]);
	strcat_s(file, ".mp4");

	// s.mp4
	char file2[52+2+4+2] = "C:/Users/Marci/Desktop/Smart City/Park Detect/vids/5.mp4";

	char *files[2] = { file, file2 };

	int numberOfThreads = 2;

	/*parallel_for_(Range(0, numberOfThreads), [&](const Range& range) {
		for (int r = range.start; r < range.end; r++)
		{
			// Process the video
			processVideo(files[r], r);
		}
	});*/

	/*std::thread first(processVideo, files[0], 0);
	std::thread second(processVideo, files[1], 1);

	first.join();
	second.join();*/
	std::thread second(processVideo, files[1], 1);
	processVideo(files[0], 0);
	second.join();

	// Close all.
	destroyAllWindows();
	return EXIT_SUCCESS;
}

void processVideo(char* videoFilename, int frameNumber) {

	Drawer drawer;
	DetectCars detectCars;
	Utils utils;
	AsphaltHandler asphaltHandler;

	Mat frame, gray;
	Mat frame2;

	//create the capture object
	VideoCapture capture(videoFilename);

	if (!capture.isOpened()) {
		//error in opening the video input
		cerr << "Unable to open video file: " << videoFilename << endl;
		exit(EXIT_FAILURE);
	}
	std::cout << "video opened: " << videoFilename << std::endl;

	// for storing input data
	char keyboard = 0;
	// When paused, pressing 'd' will jump to the next frame. FPS is not going to be correct.
	// Pressing space will pause the processing, but won't destroy it.
	// Pressing ESC will close the application.
	bool steppedPlay = false;
	bool stop = false;

	Mat grayNew = Mat::zeros(H, W, CV_8UC1);
	Mat diffImage = Mat::zeros(H, W, CV_8UC1);
	Mat imageHSV = Mat::zeros(H, W, CV_8UC3);
	Mat imageGray = Mat::zeros(H, W, CV_8UC1);
	Mat imageGrayLast = Mat::zeros(H, W, CV_8UC1);

	// Mask for the asphalt
	vector<cv::Point> points;
	points.push_back(cv::Point(522, 272));
	points.push_back(cv::Point(560, 299));
	points.push_back(cv::Point(835, 307));
	points.push_back(cv::Point(835, 280));
	Mat maskAsphalt = Mat::zeros(H, W, CV_8UC1);
	cv::fillConvexPoly(maskAsphalt, points, Scalar(255), CV_AA, 0);

	Mat *spotMasks = new Mat[NUMBER_OF_POINTS];
	
	// generate the masks for the spots
	int currX = START_POINT;
	int currIndex = 0;
	while (currX > END_POINT){
		spotMasks[currIndex] = Mat::zeros(H, W, CV_8UC1);
		shapes::Rect rect = utils.getRect(currX, 50);
		vector<cv::Point> points;
		points.push_back(rect[0]);
		points.push_back(rect[1]);
		points.push_back(rect[2]);
		points.push_back(rect[3]);
		cv::fillConvexPoly(spotMasks[currIndex], points, Scalar(255), CV_AA, 0);
		currX -= STEP_SIZE;
		currIndex++;
	}
	const int count = NUMBER_OF_POINTS;
	double percentages[count];
	for (int i = 0; i < count; i++) {
		percentages[i] = i;
	}

	const int numberOfValues = 5;
	double grayValues[numberOfValues];
	for (int i = 0; i < numberOfValues; i++) {
		grayValues[i] = 0;
	}

	std::cout << "starting video processing" << std::endl;
	while (!stop && keyboard != 27) {
		bool brek = false;
		for (int i = 0; i < 1; i++) {
			//read the current frame
			if (!capture.read(frame)) {
				cerr << "Unable to read next frame." << endl;
				brek = true;
			}
		}
		if (brek)
			break;



		double t2;
		if (logTimes) t2 = (double)getTickCount();
		cvtColor(frame, imageGray, COLOR_BGR2GRAY);
		double mean = utils.meanOfArea(imageGray, maskAsphalt);
		double percentage_of_moving_objects_at_the_asphalt = utils.calcNonZeroPixels(diffImage, maskAsphalt, false);

		
		mean = asphaltHandler.updateAsphaltColor(mean, percentage_of_moving_objects_at_the_asphalt, grayValues, numberOfValues);
		//cvtColor(frame, imageHSV, COLOR_BGR2HSV);
		//filterBlack(imageHSV, grayNew, mean);
		if (logTimes) t2 = ((double)getTickCount() - t2) / getTickFrequency();
		if (logTimes) std::cout << "asphalt proc: " << t2 * 1000 << " s" << endl;


#if LOG_TIMES
		t2 = (double)getTickCount();
#endif
		threshold(imageGray, grayNew, mean / 2, 255, THRESH_BINARY_INV);
		
		utils.getDiffImageInGray(imageGray, imageGrayLast, diffImage);

#if LOG_TIMES
		t2 = ((double)getTickCount() - t2) / getTickFrequency();
		std::cout << "image preproc: " << t2 * 1000 << " s" << endl;
#endif



#if LOG_TIMES
		t2 = (double)getTickCount();
#endif
		detectCars.calcPercentages(utils, grayNew, diffImage, spotMasks, percentages, count);
#if LOG_TIMES
		t2 = ((double)getTickCount() - t2) / getTickFrequency();
		std::cout << "calcing percentages: " << t2 * 1000 << " s" << endl;
#endif


#if LOG_TIMES
		t2 = (double)getTickCount();
#endif
		int numberOfCars = 0;
		int* positions = detectCars.calcParkinglotsStatus(frame, percentages, count, numberOfCars);
#if LOG_TIMES
		t2 = ((double)getTickCount() - t2) / getTickFrequency();
		std::cout << "calcing statuses: " << t2 * 1000 << " s" << endl;
#endif


#if LOG_TIMES
		t2 = (double)getTickCount();
#endif
		drawer.drawStatisticsOnImage(frame, percentages, count);
#if LOG_TIMES
		t2 = ((double)getTickCount() - t2) / getTickFrequency();
		std::cout << "drawing: " << t2 * 1000 << " s" << endl;
#endif


		//show the current frame
		std::string s = "Frame";
		s += std::to_string(frameNumber);
		cv::imshow(s, frame);
		
		imageGray.copyTo(imageGrayLast);

		if (frameNumber == 0){
			std::cout << "FPS: " << fps() << endl;
			std::cout << endl;
			std::cout << endl;
		}

		//get the input from the keyboard
		keyboard = (char) waitKey(1);

		stop = handleKeyPress(keyboard, steppedPlay);
		if (keyboard == 27) {
			stop = true;
		}
	}

	std::cout << "end of video processing" << std::endl;
	//delete capture object
	capture.release();
	std::cout << "video released" << std::endl;
}

/* Returns true if we should close the app (ESC pressed).
   If space is pressed, then pause the processing and wait until space is pressed again. */
bool handleKeyPress(char k, bool& steppedPlay) {
	// 32 == Space ,    27 == ESC
	if (k == 32 || steppedPlay){
		bool close = false;
		while (true) {
			k = (char)waitKey(30);
			if (k == 32) {
				steppedPlay = false;
				break;
			}
			if (k == 27) {
				close = true;
				break;
			}
			if (k == 'd'){
				steppedPlay = true;
				break;
			}
			if (k == 's'){
				//cv2.imwrite('stat.png', image);
			}
		}
		return close;
	}
	if (k == 27) {
		return true;
	}
	return false;
}


clock_t last_time;

double clockToMilliseconds(clock_t ticks) {
	// units/(units/time) => time (seconds) * 1000 = milliseconds
	return (ticks / (double)CLOCKS_PER_SEC)*1000.0;
}

double fps(){
	clock_t time_now = clock();

	clock_t deltaTime = time_now - last_time;

	double timeMillis = clockToMilliseconds(deltaTime);
	double frameRate = 0;
	if (timeMillis > 0.001)
		frameRate = 1000 / timeMillis;
	last_time = time_now;
	return frameRate;
}

/*

//get the frame number and write it on the current frame
stringstream ss;
rectangle(frame, cv::Point(10, 2), cv::Point(100, 20),
cv::Scalar(255, 255, 255), -1);
ss << capture.get(CAP_PROP_POS_FRAMES);
string frameNumberString = ss.str();
putText(frame, frameNumberString.c_str(), cv::Point(15, 15),
FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));

*/


/*
   // ######## Determine image type: rgb or bgr
   
   Mat ch1, ch2, ch3; // declare three matrices
   // "channels" is a vector of 3 Mat arrays:
   vector<Mat> channels(3);
   // split frame:
   split(frame, channels);
   channels[1] = Mat::zeros(H, W, CV_8UC1);
   channels[2] = Mat::zeros(H, W, CV_8UC1);
   merge(channels, frame);

*/