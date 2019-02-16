/*
// Only for background subtraction.
background = imread("bg.jpg", IMREAD_COLOR); // Load an image
if (background.empty())	{
	cerr << "No background image found!" << endl;
	cerr << "exiting..." << endl;
	return EXIT_FAILURE;
}
std::cout << "bckgnd opened" << std::endl;
cvtColor(background, background_gray, COLOR_BGR2GRAY);
*/


/*
//get the frame number and write it onto the current frame
stringstream ss;
rectangle(frame, cv::Point(10, 2), cv::Point(100, 20),
cv::Scalar(255, 255, 255), -1);
ss << capture.get(CAP_PROP_POS_FRAMES);
string frameNumberString = ss.str();
putText(frame, frameNumberString.c_str(), cv::Point(15, 15),
FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));
*/


/*
// Determine image type: rgb or bgr
Mat ch1, ch2, ch3; // declare three matrices
// "channels" is a vector of 3 Mat arrays:
vector<Mat> channels(3);
// split frame:
split(frame, channels);
channels[1] = Mat::zeros(H, W, CV_8UC1);
channels[2] = Mat::zeros(H, W, CV_8UC1);
merge(channels, frame);
*/