#include <iostream>
#include "Utils.h"
#include "constants.h"

using namespace std;
using namespace cv;
using namespace shapes;

uint16_t parking_zone_up_LENGTH = 13;
shapes::Point parking_zone_up[] = { shapes::Point(80, 272), shapes::Point(147, 257), shapes::Point(185, 258), shapes::Point(251, 249), shapes::Point(309, 249), shapes::Point(378, 245), shapes::Point(465, 241), shapes::Point(560, 241), shapes::Point(653, 238), shapes::Point(769, 233), shapes::Point(867, 240), shapes::Point(948, 249), shapes::Point(1016, 247) };

uint16_t parking_zone_down_LENGTH = 19;
shapes::Point parking_zone_down[] = { shapes::Point(66, 284), shapes::Point(90, 287), shapes::Point(126, 283), shapes::Point(152, 282), shapes::Point(182, 281), shapes::Point(226, 282), shapes::Point(263, 279), shapes::Point(301, 281), shapes::Point(358, 281), shapes::Point(412, 277), shapes::Point(479, 277), shapes::Point(556, 276), shapes::Point(609, 272), shapes::Point(662, 271), shapes::Point(735, 273), shapes::Point(802, 273), shapes::Point(887, 278), shapes::Point(971, 282), shapes::Point(1021, 280) };

void filterBlack(const Mat& imageHSV, Mat& imageGrayNew, double grayValue){
	Mat channels[3];
	split(imageHSV, channels);
	threshold(channels[2], imageGrayNew, grayValue/2, 255, THRESH_BINARY_INV);
}

void getDiffImageInGray(const Mat& image1, const Mat& image2, Mat& result, double thresholdVal) {
	threshold(image1 - image2, result, 10, 255, THRESH_BINARY);
}

double meanOfArea(const Mat& imageGray, const Mat& maskAsphalt) {
	Scalar mean, stddev;
	meanStdDev(imageGray, mean, stddev, maskAsphalt);
	double meanval = mean.val[0];
	return meanval;	
}


double calcNonZeroPixels(const Mat& blackAndWhite, const Mat& mask, bool percentage = true){
	int maskSize = countNonZero(mask);

	Mat blackAndWhite_masked = Mat::zeros(H, W, CV_8UC1);
	bitwise_and(blackAndWhite, blackAndWhite, blackAndWhite_masked, mask);
	int nonZeroPixels = countNonZero(blackAndWhite_masked);
	if (maskSize == 0)
		return 100;
	if (percentage)
		return 100 * nonZeroPixels / maskSize;
	else
		return nonZeroPixels;
}

double meanOfAsphalt() {
	return 0;
}

shapes::Rect getRect(double x, double width) {
	double scale = 1 - 0.8*(1030 - x) / (1030 - 60);

	double slide = (-30 * (1030 - x) / (1030 - 60)) / 2;

	uint16_t index = 1;
	while (index < parking_zone_down_LENGTH - 1 && parking_zone_down[index][0] < x - width / 2 + slide)
		index += 1;
	double rate2 = double((parking_zone_down[index][0] - x + width / 2 - slide)) / (parking_zone_down[index][0] - parking_zone_down[index - 1][0]);
	shapes::Point corner2(x - width / 2 + slide, int(parking_zone_down[index][1] + (parking_zone_down[index - 1][1] - parking_zone_down[index][1])*rate2));

	while (index < parking_zone_down_LENGTH - 1 && parking_zone_down[index][0] < x + width / 2 + slide)
		index += 1;
	double rate3 = double((parking_zone_down[index][0] - x - width / 2 - slide)) / (parking_zone_down[index][0] - parking_zone_down[index - 1][0]);
	shapes::Point corner3(x + width / 2 + slide, int(parking_zone_down[index][1] + (parking_zone_down[index - 1][1] - parking_zone_down[index][1])*rate3));


	index = 1;
	while (index < parking_zone_up_LENGTH - 1 && parking_zone_up[index][0] < x - width / 2 - slide)
		index += 1;
	double rate1 = double((parking_zone_up[index][0] - x + width / 2 + slide)) / (parking_zone_up[index][0] - parking_zone_up[index - 1][0]);
	shapes::Point corner1(x - width / 2 - slide, int(parking_zone_up[index][1] + (parking_zone_up[index - 1][1] - parking_zone_up[index][1])*rate1));

	while (index < parking_zone_up_LENGTH - 1 && parking_zone_up[index][0] < x + width / 2 - slide)
		index += 1;
	double rate4 = double((parking_zone_up[index][0] - x - width / 2 + slide)) / (parking_zone_up[index][0] - parking_zone_up[index - 1][0]);
	shapes::Point corner4(x + width / 2 - slide, int(parking_zone_up[index][1] + (parking_zone_up[index - 1][1] - parking_zone_up[index][1])*rate4));
	
	shapes::Rect rect = shapes::Rect(corner1, corner2, corner3, corner4);
	return rect;

};

void draw_rectangle_on_image(Mat& image, shapes::Rect rect, Scalar color, int width = 2) {
	line(image, rect[0], rect[1], color, width);
	line(image, rect[1], rect[2], color, width);
	line(image, rect[2], rect[3], color, width);
	line(image, rect[3], rect[0], color, width);
};

void drawOnRectangles(Mat& image, int start, int diff) {
	double currX = 950 - start * STEP_SIZE;
	int i = start;
	if (i < 0)
		i = 0;
	if (diff < 5)
		diff = 5;
	while (currX > 60) {
		shapes::Rect rect = getRect(currX, 50);
		currX -= STEP_SIZE * diff;
		draw_rectangle_on_image(image, rect, Scalar(0, 0, 255));
		i += diff;
	}
}

