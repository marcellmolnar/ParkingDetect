#include "detectCars.h"

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
		double percentage = calcNonZeroPixels(thresh, spotMasks[i], true);
		double percentage_of_moving_objects = 2; // calcNonZeroPixels(diffImage, mask = maskRectangles[i], percentage = true);
		newPercentages[i] = percentage;
		if (percentage_of_moving_objects < 5)
			canRefresh[i] = true;
		else
			canRefresh[i] = false;
		currX -= STEP_SIZE;
		i += 1;
	}
	for (int i = 0; i < NUMBER_OF_POINTS; i++) {
		bool refresh = true;
		if (i > 0 and canRefresh[i - 1] == false)
			refresh = false;
		if (i > 1 and canRefresh[i - 2] == false)
			refresh = false;
		if (i > 2 and canRefresh[i - 3] == false)
			refresh = false;
		if (i > 3 and canRefresh[i - 4] == false)
			refresh = false;

		if (i < NUMBER_OF_POINTS - 1 and canRefresh[i + 1] == false)
			refresh = false;
		if (i < NUMBER_OF_POINTS - 2 and canRefresh[i + 2] == false)
			refresh = false;
		if (i < NUMBER_OF_POINTS - 3 and canRefresh[i + 3] == false)
			refresh = false;
		if (i < NUMBER_OF_POINTS - 4 and canRefresh[i + 4] == false)
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
	
	int* parkingCarsNow = new int[20]();
	int position = 0;

	double last_percentage = 100;
	bool ascending = false;

	int currX = START_POINT;
	i = 0;
	double distance_from_last_rect = 100;	//some big random number, ensuring the first car will be detected
	while (currX > END_POINT) {
		double scale = 1 - 0.80*(START_POINT - currX) / (START_POINT - END_POINT);
		double percentage = percentagesToDraw[i];
		if (percentage < last_percentage && ascending) {
			ascending = false;
			if (last_percentage > 15 and distance_from_last_rect > 70 * scale) {
				int width = int(80 * scale);
				//rect = getRect(currX, width);
				//parkingCarsNow.append(Parking_lot(rect));
				parkingCarsNow[position] = currX;
				position += 1; if (position == 20) position = 19;
				circle(image, cv::Point(currX + STEP_SIZE, graphMinY - last_percentage * 3), 4, Scalar(0, 255, 255), 4);
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

	numberOfCars = position;
	return parkingCarsNow;
}