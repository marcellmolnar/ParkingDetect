#include "video_processor.h"
#define LOG_TIMES false

using namespace cv;
using namespace std;

bool handleKeyPress(char k, bool& steppedPlay);
double fps(clock_t t_start);

void processVideo(char* videoFilename, int frameNumber) {

	Drawer drawer;
	DetectCars detectCars;
	Utils utils;
	AsphaltHandler asphaltHandler;
	CarHandler carHandler;

	Mat frame;

	//create the capture object
	VideoCapture capture(videoFilename);

	if (!capture.isOpened()) {
		//error in opening the video input
		cerr << "Unable to open video file: " << videoFilename << endl;
		exit(EXIT_FAILURE);
	}
	cout << "video opened: " << videoFilename << endl;

	// For storing input data.
	char keyboard = 0;
	/* When paused, pressing 'd' will jump to the next frame. FPS is not going to be correct.
	 * Pressing space will pause the processing, but won't destroy it.
	 * Pressing ESC will close the application, regardless it's running or paused. */
	// In paused mode, it indicates that we should process 1 frame.
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

	/* Array holding all the masks we use when calculating the percentages. 
	It is faster if we store them, and not creating them at every frame. */
	Mat *spotMasks = new Mat[NUMBER_OF_POINTS];
	int currX = START_POINT;
	int currIndex = 0;
	// Generating the masks for the spots.
	while (currX > END_POINT) {
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

	double percentages[NUMBER_OF_POINTS];
	for (int i = 0; i < NUMBER_OF_POINTS; i++) {
		percentages[i] = 0;
	}

	const int numberOfValues = 5;
	double grayValues[numberOfValues];
	for (int i = 0; i < numberOfValues; i++) {
		grayValues[i] = 0;
	}

	cout << "starting video processing" << endl;
	while (!stop && keyboard != 27) {
		clock_t t_start = clock();

		// For speeding up. i < N (N > 0) will will causing N times faster fps.
		bool brek = false;
		for (int i = 0; i < 1; i++) {
			//read the current frame
			if (!capture.read(frame)) {
				cerr << "Unable to read next frame." << endl;
				brek = true;
			}
		}
		if (brek) break;

#if LOG_TIMES
		// Storing each processing step's start time.
		double t2;
		t2 = (double) getTickCount();
#endif

		cvtColor(frame, imageGray, COLOR_RGB2GRAY);
		double mean = utils.meanOfArea(imageGray, maskAsphalt);
		double number_of_moving_pixels_at_the_asphalt = utils.calcNonZeroPixels(diffImage, maskAsphalt, false);


		mean = asphaltHandler.updateAsphaltColor(mean, number_of_moving_pixels_at_the_asphalt, grayValues, numberOfValues);
		//cvtColor(frame, imageHSV, COLOR_BGR2HSV);
		//filterBlack(imageHSV, grayNew, mean);
#if LOG_TIMES
		t2 = ((double) getTickCount() - t2) / getTickFrequency();
		cout << "asphalt proc: " << t2 * 1000 << " s" << endl;
#endif


#if LOG_TIMES
		t2 = (double)getTickCount();
#endif
		//threshold(imageGray, grayNew, mean / 2, 255, THRESH_BINARY_INV);
		cvtColor(frame, imageHSV, COLOR_BGR2HSV);
		utils.filterBlack(imageHSV, grayNew, mean / 2);
		imshow("hi", grayNew);

		utils.getDiffImageInGray(imageGray, imageGrayLast, diffImage);

#if LOG_TIMES
		t2 = ((double) getTickCount() - t2) / getTickFrequency();
		cout << "image preproc: " << t2 * 1000 << " s" << endl;
#endif



#if LOG_TIMES
		t2 = (double) getTickCount();
#endif
		detectCars.calcPercentages(utils, grayNew, diffImage, spotMasks, percentages, NUMBER_OF_POINTS);
#if LOG_TIMES
		t2 = ((double) getTickCount() - t2) / getTickFrequency();
		cout << "calcing percentages: " << t2 * 1000 << " s" << endl;
#endif


#if LOG_TIMES
		t2 = (double) getTickCount();
#endif
		int numberOfCars = 0;
		int* positions = detectCars.calcParkinglotsStatus(frame, percentages, NUMBER_OF_POINTS, numberOfCars);
#if LOG_TIMES
		t2 = ((double) getTickCount() - t2) / getTickFrequency();
		cout << "calcing statuses: " << t2 * 1000 << " s" << endl;
#endif
		carHandler.matchNewCars(positions, numberOfCars);


#if LOG_TIMES
		t2 = (double) getTickCount();
#endif
		drawer.drawStatisticsOnImage(frame, percentages, NUMBER_OF_POINTS);
#if LOG_TIMES
		t2 = ((double) getTickCount() - t2) / getTickFrequency();
		cout << "drawing: " << t2 * 1000 << " s" << endl;
#endif


		//show the current frame
		string s = "Frame";
		s += to_string(frameNumber);
		imshow(s, frame);

		imageGray.copyTo(imageGrayLast);

		if (frameNumber == 0) {
			//cout << "FPS: " << fps(t_start) << endl;
		}
		//cout << endl;

		//get the input from the keyboard
		keyboard = (char)waitKey(1);

		stop = handleKeyPress(keyboard, steppedPlay);
		if (keyboard == 27) {
			stop = true;
		}
	}

	cout << "end of video processing" << endl;
	//delete capture object
	capture.release();
	cout << "video released" << endl;
}


/* Returns true if we should close the app (ESC pressed).
   If space is pressed, then pause the processing and wait until space is pressed again. */
bool handleKeyPress(char k, bool& steppedPlay) {
	// 32 == Space ,    27 == ESC
	if (k == 32 || steppedPlay) {
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
			if (k == 'd') {
				steppedPlay = true;
				break;
			}
			if (k == 's') {
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


double clockToMilliseconds(clock_t ticks) {
	return (ticks / (double)CLOCKS_PER_SEC)*1000.0;
}

double fps(clock_t t_start) {
	clock_t time_now = clock();

	clock_t deltaTime = time_now - t_start;

	double timeMillis = clockToMilliseconds(deltaTime);
	double frameRate = 0;
	if (timeMillis > 0.001)
		frameRate = 1000 / timeMillis;
	return frameRate;
}