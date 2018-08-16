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
//own
#include "Utils.h"


using namespace cv;
using namespace std;


Mat frame, gray;
Mat background;
Mat background_gray;

uint16_t H = 720;
uint16_t W = 1280;

bool steppedPlay = false;

void processVideo(char* videoFilename);
bool handleKeyPress(char k);
void filterBlack(const Mat& image, Mat& imageGrayNew, double grayValue);

int main(int argc, char* argv[])
{
	std::cout << "app started" << std::endl;
	steppedPlay = false;

	Parking_lot p(shapes::Point(522, 272), shapes::Point(560, 299), shapes::Point(835, 307), shapes::Point(835, 280));
		
	if (argc != 2) {
		cerr << "Incorret input list!" << endl;
		cerr << "exiting..." << endl;
		return EXIT_FAILURE;
	}

	background = imread("bg.jpg", IMREAD_COLOR); // Load an image
	if (background.empty())
	{
		cerr << "No background image found!" << endl;
		cerr << "exiting..." << endl;
		return EXIT_FAILURE;
	}
	std::cout << "bckgnd opened" << std::endl;
	cvtColor(background, background_gray, COLOR_BGR2GRAY);

	namedWindow("Frame");
	std::cout << "window created" << std::endl;

	processVideo(argv[1]);

	destroyAllWindows();
	return EXIT_SUCCESS;
}

void processVideo(char* videoFilename) {
	//create the capture object
	VideoCapture capture(videoFilename);

	if (!capture.isOpened()) {
		//error in opening the video input
		cerr << "Unable to open video file: " << videoFilename << endl;
		exit(EXIT_FAILURE);
	}
	std::cout << "video opened: " << videoFilename << std::endl;

	//read input data
	char keyboard = 0;
	bool stop = false;
	
	Mat grayNew = Mat::zeros(H, W, CV_8UC1);
	Mat imageHSV = Mat::zeros(H, W, CV_8UC3);
	
	std::cout << "starting video processing" << std::endl;
	while (!stop && keyboard != 27) {
		//read the current frame
		if (!capture.read(frame)) {
			cerr << "Unable to read next frame." << endl;
			break;
		}
		
		cvtColor(frame, imageHSV, COLOR_BGR2HSV);
		filterBlack(imageHSV, grayNew, 168);

		//std::cout << "start drawing" << std::endl;
		//drawOnRectangles(frame, 0, 5);
		
		//show the current frame
		imshow("Frame", grayNew);
		std::cout << "image showed" << std::endl;

		//get the input from the keyboard
		keyboard = (char) waitKey(30);
		std::cout << "keyboard read" << std::endl;

		stop = handleKeyPress(keyboard);
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
bool handleKeyPress(char k) {
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