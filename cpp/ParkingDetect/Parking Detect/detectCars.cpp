#include "detectCars.h"

using namespace cv;
using namespace std;

double *percentagesToDraw = new double[NUMBER_OF_POINTS]();
bool *canRefresh = new bool[NUMBER_OF_POINTS]();

void DetectCars::calcPercentages(const Utils& utils, const Mat& thresh, const Mat& diffImage, Mat* spotMasks, double* percentages, int count) {
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


	parallel_for_(Range(0, NUMBER_OF_POINTS), [&](const Range& range) {
		for (int r = range.start; r < range.end; r++)
		{
			// Calc the number of the non-zero-valued pixels in the current spot in percentage.
			double percentage = utils.calcNonZeroPixels(thresh, spotMasks[r], true);
			double percentage_of_moving_objects = utils.calcNonZeroPixels(diffImage, spotMasks[r], true);
			newPercentages[r] = percentage;
			// Don't refresh when there is some moving object..
			if (percentage_of_moving_objects < 2)
				canRefresh[r] = true;
			else
				canRefresh[r] = false;
			currX -= STEP_SIZE;
			i += 1;
		}
	});


	/*while (currX > END_POINT) {
		// Calc the number of the non-zero-valued pixels in the current spot in percentage.
		double percentage = utils.calcNonZeroPixels(thresh, spotMasks[i], true);
		double percentage_of_moving_objects = utils.calcNonZeroPixels(diffImage, spotMasks[i], true);
		newPercentages[i] = percentage;
		// Don't refresh when there is some moving object..
		if (percentage_of_moving_objects < 5)
			canRefresh[i] = true;
		else
			canRefresh[i] = false;
		currX -= STEP_SIZE;
		i += 1;
	}*/

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


int* DetectCars::calcParkinglotsStatus(Mat& image, double* percentages, int count, int& numberOfCars){
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
	// change percentages to the averaged ones
	percentages = percentagesToDraw;
	
	// x coordinates of the parking cars (list of max 20)
	int* parkingCarsNow = new int[20]();
	// number of parking cars
	int position = 0;

	double last_percentage = percentages[0]+0.1;
	bool ascending = (percentages[0] > 10);

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
			if (last_percentage > THRESH_2_DETECT_CAR && distance_from_last_rect > MIN_DISTANCE_BETWEEN_CARS * rectWidthScale) {

				int width = int(80 * rectWidthScale);
				//rect = getRect(currX, width);
				//parkingCarsNow.append(Parking_lot(rect));
				parkingCarsNow[position] = currX;
				position += 1; if (position == 20) position = 19; // don't try to write a 21st 
				// draw a point at this car's position
				if (currX == START_POINT) {
					circle(image, cv::Point(currX, graphMinY - last_percentage * scale), circleRadius, COLOR_YELLOW, -1);
				}
				else {
					if (currX > MIN_COORDINATE)
					circle(image, cv::Point(currX + STEP_SIZE, graphMinY - last_percentage * scale), circleRadius, COLOR_YELLOW, -1);
				}
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
	// save the number of detected cars
	numberOfCars = position;
	// return with the array of the parking cars' x coordinates
	return parkingCarsNow;
}