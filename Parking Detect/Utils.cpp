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

using namespace cv;

void filterBlack(const Mat& image, const Mat& imageGray, Mat& imageGrayNew, double grayValue){
	Mat hsv;
	cvtColor(image, hsv, COLOR_BGR2HSV);
	imageGrayNew;

	int channels = imageGray.channels();
	int nRows = imageGray.rows;
	int nCols = imageGray.cols;
	int i, j;
	uchar* p;
	for (i = 0; i < nRows; ++i)	{
		for (j = 0; j < nCols; ++j)	{
			p = imageGrayNew.ptr<uchar>(i);
			if (hsv.at<cv::Vec3b>(i, j)[2] > grayValue) {
				p[j] = 0;
			}
			else {
				p[j] = 255;
			}
			
		}
	}


	/*mask = np.where(hsv[:, : , 2] > grayValue, 0, 255)

	hsv[:, : , 2] = mask
	imageGrayNew[:] = np.where(mask > 120, 255, 0)
	
	*/

	return;
}