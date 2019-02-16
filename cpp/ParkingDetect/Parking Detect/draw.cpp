#include "draw.h"

using namespace cv;
using namespace std;

void Drawer::drawStatisticsOnImage(Mat& image, double percentages[], int count) {
	if (count != NUMBER_OF_POINTS) {
		cerr << "Unable to read next frame." << endl;
		return;
	}

	// scale for converting the percentage values to screen values
	double scale = (graphMinY - graphMaxY) / 100;
	double last_percentage = percentages[0];
	int currX = START_POINT - STEP_SIZE;

	// drawing the actual graph
	for (int i = 1; i < NUMBER_OF_POINTS; i++){
		double percentage = percentages[i];
		Scalar color;
		if (i % 2 == 0)
			color = COLOR_GREEN;
		else
			color = COLOR_RED;
		line(image, Point(currX + STEP_SIZE, graphMinY - last_percentage * scale), Point(currX, graphMinY - percentage * scale), color, 2);
		last_percentage = percentage;
		currX -= STEP_SIZE;
	}
	//return; // fps drops when drawing the coordinate system

	double gridHeight = (graphMinY - graphMaxY) / 5;
	// vertical grid
	for (int i = 0; i < 11; i++) {
		line(image, Point(START_POINT - i * 100, graphMaxY), Point(START_POINT - i * 100, graphMinY), COLOR_WHITE, 1);
		char str[5];
		strcpy(str, "");
		char space;
		if (START_POINT - i * 100 < 100)
			strcat(str, " ");
		std::ostringstream oss;
		oss << str << (START_POINT - i * 100);
		putText(image, oss.str(), Point(START_POINT - i * 100 - 30, graphMinY + 40), 1, 2, COLOR_YELLOW, 2);
	}
	// vertical axis
	line(image, Point(50, graphMaxY), Point(50, graphMinY), COLOR_BLUE, 2);
	line(image, Point(40, graphMaxY + 20), Point(50, graphMaxY), COLOR_BLUE, 2);
	line(image, Point(60, graphMaxY + 20), Point(50, graphMaxY), COLOR_BLUE, 2);
	putText(image, "Percentage [%]", Point(40, graphMaxY - 20), 1, 2, COLOR_YELLOW, 2);


	// horizontal grid
	for (int i = 0; i < 6; i++) {
		line(image, Point(50, graphMinY - i * gridHeight), Point(START_POINT, graphMinY - i * gridHeight), COLOR_WHITE, 1);
		char str[5];
		strcpy(str, "");
		char space;
		if (START_POINT - i * 100 < 100)
			strcat(str, " ");
		std::ostringstream oss;
		oss << str << (i * 20);
		putText(image, oss.str(), Point(5, graphMinY - i * gridHeight + 10), 1, 2, COLOR_YELLOW, 2);
	}

	// horizontal axis
	line(image, Point(50, graphMinY), Point(START_POINT, graphMinY), COLOR_BLUE, 2);
	line(image, Point(START_POINT - STEP_SIZE, graphMinY - STEP_SIZE), Point(START_POINT, graphMinY), COLOR_BLUE, 2);
	line(image, Point(START_POINT - STEP_SIZE, graphMinY + STEP_SIZE), Point(START_POINT, graphMinY), COLOR_BLUE, 2);
	putText(image, "X coord.", Point(START_POINT - 20, graphMinY + 40), 1, 2, COLOR_YELLOW, 2);

	// border for detection
	line(image, Point(MIN_COORDINATE, 0), Point(MIN_COORDINATE, H), COLOR_RED, 4);

}