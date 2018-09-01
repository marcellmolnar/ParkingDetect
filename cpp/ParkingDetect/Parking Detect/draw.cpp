#include "draw.h"

// BGR colors
const Scalar COLOR_BLACK = Scalar(0, 0, 0);
const Scalar COLOR_WHITE = Scalar(255, 255, 255);
const Scalar COLOR_BLUE = Scalar(255, 0, 0);
const Scalar COLOR_RED = Scalar(0, 0, 255);
const Scalar COLOR_GREEN = Scalar(0, 255, 0);
const Scalar COLOR_YELLOW = Scalar(0, 255, 255);


void drawStatisticsOnImage(Mat& image, double percentages[], int count) {
	const int minY = 500;
	const int maxY = 200;
	if (count != NUMBER_OF_POINTS) {
		cerr << "Unable to read next frame." << endl;
		return;
	}
	double scale = (minY - maxY) / 100;
	double gridHeight = (minY - maxY) / 5;
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
		line(image, Point(currX + STEP_SIZE, minY - last_percentage * scale), Point(currX, minY - percentage * scale), color, 2);
		last_percentage = percentage;
		currX -= 10;
	}

	for (int i = 1; i < 11; i++) {
		line(image, Point(START_POINT - i * 100, maxY), Point(START_POINT - i * 100, minY), (255,255,255), 1);
		char str[5];
		strcpy(str, "");
		char space;
		if (START_POINT - i * 100 < 100)
			strcat(str, " ");
		std::ostringstream oss;
		oss << str << (START_POINT - i * 100);
		putText(image, oss.str(), Point(START_POINT - i * 100 - 30, minY + 40), 1, 2, COLOR_YELLOW, 3);
	}
	// vertical axis
	line(image, Point(50, maxY), Point(50, minY), COLOR_BLUE, 2);
	line(image, Point(40, maxY + 20), Point(50, maxY), COLOR_BLUE, 2);
	line(image, Point(60, maxY + 20), Point(50, maxY), COLOR_BLUE, 2);
	putText(image, "Percentage [%]", Point(40, maxY - 20), 1, 2, COLOR_YELLOW, 3);
	for (int i = 0; i < 6; i++) {
		line(image, Point(50, minY - i * gridHeight), Point(START_POINT, minY - i * gridHeight), COLOR_WHITE, 1);
		char str[5];
		strcpy(str, "");
		char space;
		if (START_POINT - i * 100 < 100)
			strcat(str, " ");
		std::ostringstream oss;
		oss << str << (i * 20);
		putText(image, oss.str(), Point(5, minY - i * gridHeight + 10), 1, 2, COLOR_YELLOW, 3);
	}

	// horizontal axis
	line(image, Point(50, minY), Point(START_POINT, minY), COLOR_BLUE, 2);
	line(image, Point(START_POINT - 10, minY - 10), Point(START_POINT, minY), COLOR_BLUE, 2);
	line(image, Point(START_POINT - 10, minY + 10), Point(START_POINT, minY), COLOR_BLUE, 2);
	putText(image, "X coord.", Point(START_POINT - 20, minY + 40), 1, 2, COLOR_YELLOW, 3);

}