//opencv
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
//C
#include <cstdio>
#include <cstdlib>
//C++
#include <iostream>
#include <sstream>
#include <thread>
//own
#include "AsphaltHandler.h"
#include "constants.h"
#include "detectCars.h"
#include "draw.h"
#include "Utils.h"
#include "video_processor.h"
//TBB (Threading Building Blocks) for threading
#include "tbb/tbb.h"

#define NUM_OF_CAMERAS 1

using namespace cv;
using namespace std;


int main(int argc, char* argv[]) {

	//Parking_spot p(shapes::Point(522, 272), shapes::Point(560, 299), shapes::Point(835, 307), shapes::Point(835, 280));
		
	if (argc != 2) {
		cerr << "Incorret input list! You must provide the video's number!" << endl;
		cerr << "exiting..." << endl;
		return EXIT_FAILURE;
	}

	// create window
	namedWindow("Frame0");
	std::cout << "window(s) created" << std::endl;

	// Name and full path of the video.
	char file[54+2+4+2];
	strcpy_s(file, "C:/Users/Marci/Desktop/Smart City/vids/");
	strcat_s(file, argv[1]);
	strcat_s(file, ".mp4");

#if (NUM_OF_CAMERAS == 1)
	processVideo(file, 0);
#endif

#if (NUM_OF_CAMERAS == 2)
	char file2[54+2+4+2] = "C:/Users/Marci/Desktop/Smart City/vids/5.mp4";
	char *files[2] = { file, file2 };

	int numberOfThreads = 2;

	/*parallel_for_(Range(0, numberOfThreads), [&](const Range& range) {
		for (int r = range.start; r < range.end; r++) {
			// Process the video
			processVideo(files[r], r);
		}
	});*/

	/*std::thread first(processVideo, files[0], 0);
	std::thread second(processVideo, files[1], 1);

	first.join();
	second.join();*/

	std::thread second(processVideo, files[1], 1, logTimes);
	processVideo(files[0], 0, logTimes);
	second.join();
#endif

	// Close all.
	destroyAllWindows();
	return EXIT_SUCCESS;
}

