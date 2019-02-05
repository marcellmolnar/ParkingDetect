#include "detectCars.h"

using namespace cv;
using namespace std;

double *percentagesToDraw = new double[NUMBER_OF_POINTS]();
bool *canRefresh = new bool[NUMBER_OF_POINTS]();

void calcPercentages(const Mat& thresh, const Mat& diffImage, Mat* spotMasks, double* percentages, int count) {
	if (count != NUMBER_OF_POINTS) {
		cerr << "Wrong number of percentages in: 'calcPercentages'" << endl;
		for (int i = 0; i < count; i++) {
			percentages[i] = 0;
		}
		return;
	}
	int currX = START_POINT;
	int i = 0;
	double *newPercentages = new double[NUMBER_OF_POINTS]();
	while (currX > END_POINT) {
		// Calc the number of the non-zero-valued pixels in the current spot in percentage.
		double percentage = calcNonZeroPixels(thresh, spotMasks[i], true);
		double percentage_of_moving_objects = calcNonZeroPixels(diffImage, spotMasks[i], true);
		newPercentages[i] = percentage;
		// Don't refresh when there is some moving object..
		if (percentage_of_moving_objects < 5)
			canRefresh[i] = true;
		else
			canRefresh[i] = false;
		currX -= STEP_SIZE;
		i += 1;
	}

	// Refresh only when there is no movement at the adjacent spots.
	for (int i = 0; i < NUMBER_OF_POINTS; i++) {
		bool refresh = true;
		if (i > 0 && canRefresh[i - 1] == false)
			refresh = false;
		if (i > 1 && canRefresh[i - 2] == false)
			refresh = false;
		if (i > 2 && canRefresh[i - 3] == false)
			refresh = false;
		if (i > 3 && canRefresh[i - 4] == false)
			refresh = false;

		if (i < NUMBER_OF_POINTS - 1 && canRefresh[i + 1] == false)
			refresh = false;
		if (i < NUMBER_OF_POINTS - 2 && canRefresh[i + 2] == false)
			refresh = false;
		if (i < NUMBER_OF_POINTS - 3 && canRefresh[i + 3] == false)
			refresh = false;
		if (i < NUMBER_OF_POINTS - 4 && canRefresh[i + 4] == false)
			refresh = false;

		if (refresh)
			percentages[i] = newPercentages[i];
	}

}


int* calcParkinglotsStatus(Mat& image, double* percentages, int count, int& numberOfCars){
	if (count != NUMBER_OF_POINTS) {
		cerr << "Wrong number of percentages in: 'calcParkinglotsStatus'" << endl;
		for (int i = 0; i < count; i++) {
			percentages[i] = 0;
		}
		numberOfCars = 0;
		return nullptr;
	}

	int i = 1;
	// averaging
	while (i < NUMBER_OF_POINTS - 1){
		percentagesToDraw[i] = (percentages[i - 1] + percentages[i] + percentages[i + 1]) / 3;
		i += 1;
		percentagesToDraw[i] = percentages[i];
	}
	
	// x coordinates of the parking cars
	int* parkingCarsNow = new int[20]();
	// number of parking cars
	int position = 0;

	double last_percentage = 100;
	bool ascending = false;

	int currX = START_POINT;
	i = 0;
	double distance_from_last_rect = 100;	//some big random number, ensuring the first car will be detected
	double scale = (graphMinY - graphMaxY) / 100;
	while (currX > END_POINT) {
		// scale for descend the witdh of the rectangles because the farther we look, the smaller are the cars
		double rectWidthScale = 1 - 0.80*(START_POINT - currX) / (START_POINT - END_POINT);
		double percentage = percentages[i];
		// it can be a car when it is a local maximum on the graph (see image after the drawStatisticsOnImage() function called)
		if (percentage < last_percentage && ascending) {
			ascending = false;
			// if we are above some threshold and far enough from the last car then it is another car
			if (last_percentage > 5 && distance_from_last_rect > 50 * rectWidthScale) {
				int width = int(80 * rectWidthScale);
				//rect = getRect(currX, width);
				//parkingCarsNow.append(Parking_lot(rect));
				parkingCarsNow[position] = currX;
				position += 1; if (position == 20) position = 19; // don't try to write a 21st 
				// draw a point at this car's position
				circle(image, cv::Point(currX + STEP_SIZE, graphMinY - percentages[i-1] * scale), circleRadius, COLOR_YELLOW, circleThickness);
				distance_from_last_rect = 0;
			}
		}
		if (percentage >= last_percentage)
			ascending = true;
		last_percentage = percentage;
		currX -= STEP_SIZE;
		i += 1;
		distance_from_last_rect += STEP_SIZE;
	}

	// change percentages to the averaged ones
	percentages = percentagesToDraw;
	// save the number of detected cars
	numberOfCars = position;
	// return with the array of the parking cars' x coordinates
	return parkingCarsNow;
}