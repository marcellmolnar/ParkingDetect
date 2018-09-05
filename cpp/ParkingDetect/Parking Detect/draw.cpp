#include "draw.h"

// BGR colors
const Scalar COLOR_BLACK = Scalar(0, 0, 0);
const Scalar COLOR_WHITE = Scalar(255, 255, 255);
const Scalar COLOR_BLUE = Scalar(255, 0, 0);
const Scalar COLOR_RED = Scalar(0, 0, 255);
const Scalar COLOR_GREEN = Scalar(0, 255, 0);
const Scalar COLOR_YELLOW = Scalar(0, 255, 255);


void drawStatisticsOnImage(Mat& image, double percentages[], int count) {
	if (count != NUMBER_OF_POINTS) {
		cerr << "Unable to read next frame." << endl;
		return;
	}
	double scale = (graphMinY - graphMaxY) / 100;
	double gridHeight = (graphMinY - graphMaxY) / 5;
	int currX = START_POINT - STEP_SIZE;
	double last_percentage = percentages[0];
	double avg = 0;
	for (int i = 0; i < NUMBER_OF_POINTS; i++){
		double percentage = percentages[i];
		double percentage2 = percentages[i];
		avg += percentage;
		Scalar color = COLOR_BLUE;
		if (last_percentage > percentage)
			color = COLOR_RED;
		else
			color = COLOR_GREEN;
		if ((currX % 20) == 0)
			color = COLOR_GREEN;
		else
			color = COLOR_RED;
		line(image, Point(currX + STEP_SIZE, graphMinY - last_percentage * scale), Point(currX, graphMinY - percentage * scale), color, 2);
		last_percentage = percentage;
		currX -= 10;
	}

	for (int i = 1; i < 11; i++) {
		line(image, Point(START_POINT - i * 100, graphMaxY), Point(START_POINT - i * 100, graphMinY), (255,255,255), 1);
		char str[5];
		strcpy(str, "");
		char space;
		if (START_POINT - i * 100 < 100)
			strcat(str, " ");
		std::ostringstream oss;
		oss << str << (START_POINT - i * 100);
		putText(image, oss.str(), Point(START_POINT - i * 100 - 30, graphMinY + 40), 1, 2, COLOR_YELLOW, 3);
	}
	// vertical axis
	line(image, Point(50, graphMaxY), Point(50, graphMinY), COLOR_BLUE, 2);
	line(image, Point(40, graphMaxY + 20), Point(50, graphMaxY), COLOR_BLUE, 2);
	line(image, Point(60, graphMaxY + 20), Point(50, graphMaxY), COLOR_BLUE, 2);
	putText(image, "Percentage [%]", Point(40, graphMaxY - 20), 1, 2, COLOR_YELLOW, 3);
	for (int i = 0; i < 6; i++) {
		line(image, Point(50, graphMinY - i * gridHeight), Point(START_POINT, graphMinY - i * gridHeight), COLOR_WHITE, 1);
		char str[5];
		strcpy(str, "");
		char space;
		if (START_POINT - i * 100 < 100)
			strcat(str, " ");
		std::ostringstream oss;
		oss << str << (i * 20);
		putText(image, oss.str(), Point(5, graphMinY - i * gridHeight + 10), 1, 2, COLOR_YELLOW, 3);
	}

	// horizontal axis
	line(image, Point(50, graphMinY), Point(START_POINT, graphMinY), COLOR_BLUE, 2);
	line(image, Point(START_POINT - 10, graphMinY - 10), Point(START_POINT, graphMinY), COLOR_BLUE, 2);
	line(image, Point(START_POINT - 10, graphMinY + 10), Point(START_POINT, graphMinY), COLOR_BLUE, 2);
	putText(image, "X coord.", Point(START_POINT - 20, graphMinY + 40), 1, 2, COLOR_YELLOW, 3);

}