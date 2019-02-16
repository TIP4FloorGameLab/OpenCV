//motionTracking.cpp

//Written by  Kyle Hounslow, January 2014

//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software")
//, to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
//and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

//The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
//IN THE SOFTWARE.

#include <opencv2/opencv.hpp>

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>

using namespace std;
using namespace cv;

const static int SENSITIVITY_VALUE = 80;

const static int BLUR_SIZE = 10;

int theObject[2] = { 0,0 };
Rect objectBoundingRectangle = Rect(0, 0, 0, 0);


string intToString(int number) {

	std::stringstream ss;
	ss << number;
	return ss.str();
}

void searchForMovement(Mat thresholdImage, Mat &cameraFeed) {
	bool objectDetected = false;
	Mat temp;
	thresholdImage.copyTo(temp);

	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;

	findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	findContours(temp, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	if (contours.size() > 0)objectDetected = true;
	else objectDetected = false;
	int x{ 0 }, y{ 0 };

	if (objectDetected) {
		vector< vector<Point> > largestContourVec;
		largestContourVec.push_back(contours.at(contours.size() - 1));

		objectBoundingRectangle = boundingRect(largestContourVec.at(0));
		int xpos = objectBoundingRectangle.x + objectBoundingRectangle.width / 2;
		int ypos = objectBoundingRectangle.y + objectBoundingRectangle.height / 2;

		theObject[0] = xpos, theObject[1] = ypos;

		x = theObject[0];
		y = theObject[1];

		//ROI 설정 ( y기준 )
		Rect rect(0, y - 25, cameraFeed.size().width, 70);
		rectangle(cameraFeed, rect, Scalar(255, 0, 0),2);
		// ROI 복제 
		Mat ROI = cameraFeed.clone();

		// ROI 크롭
		ROI = ROI.rowRange(rect.y, rect.y + 70);

		// show
		imshow("ROI", ROI);
	}


	circle(cameraFeed, Point(x, y), 20, Scalar(0, 255, 0), 1);
	line(cameraFeed, Point(x, y), Point(x, y - 25), Scalar(0, 255, 0), 1);
	line(cameraFeed, Point(x, y), Point(x, y + 25), Scalar(0, 255, 0), 1);
	line(cameraFeed, Point(x, y), Point(x - 25, y), Scalar(0, 255, 0), 1);
	line(cameraFeed, Point(x, y), Point(x + 25, y), Scalar(0, 255, 0), 1);
	putText(cameraFeed, "Tracking object at (" + intToString(x) + "," + intToString(y) + ")", Point(x, y), 1, 1, Scalar(255, 0, 0), 2);



}

void DrawROI()
{
	//Mat imageROI;
	//imageROI = capture()
}

int main() {

	bool objectDetected = false;
	bool debugMode = false;
	bool trackingEnabled = true;
	bool pause = false;

	Mat frame1, frame2;
	Mat grayImage1, grayImage2;
	Mat differenceImage;
	Mat thresholdImage;

	VideoCapture capture;

	//---캠으로 할 때----
	//bool update_bg_model = true;

	//int deviceID = 0;					   // 0 = open default camera
	//int apiID = cv::CAP_ANY;      // 0 = autodetect default API

	//capture.open(deviceID + apiID);
	//
	//---비디오로 할 때---
		capture.open("../video/MAH00572.mp4");
	if (!capture.isOpened()) {
		cout << "ERROR ACQUIRING VIDEO FEED\n";
		return -1;
	}


	while (1) {
		//---캠으로 할 때----
		//while(true){
		//---비디오로 할 때---
		while (capture.get(cv::CAP_PROP_POS_FRAMES) < capture.get(cv::CAP_PROP_FRAME_COUNT) - 1) {
			
			capture.read(frame1);
			cv::cvtColor(frame1, grayImage1, COLOR_BGR2GRAY);

			capture.read(frame2);
			cv::cvtColor(frame2, grayImage2, COLOR_BGR2GRAY);

			cv::absdiff(grayImage1, grayImage2, differenceImage);
			cv::threshold(differenceImage, thresholdImage, SENSITIVITY_VALUE, 255, THRESH_BINARY);

			if (debugMode == true) {
				cv::imshow("Difference Image", differenceImage);
				cv::imshow("Threshold Image", thresholdImage);
			}
			else {
				cv::destroyWindow("Difference Image");
				cv::destroyWindow("Threshold Image");
			}

			cv::blur(thresholdImage, thresholdImage, cv::Size(BLUR_SIZE, BLUR_SIZE));
			cv::threshold(thresholdImage, thresholdImage, SENSITIVITY_VALUE, 255, THRESH_BINARY);
			if (debugMode == true) {
				imshow("Final Threshold Image", thresholdImage);
			}
			else {
				cv::destroyWindow("Final Threshold Image");
			}

			if (trackingEnabled) {
				searchForMovement(thresholdImage, frame1);
			}



			imshow("Frame1", frame1);


			///------------------------------------------
	
			switch (waitKey(10)) {

			case 27: //'esc' key has been pressed, exit program.
				return 0;
			case 116: //'t' has been pressed. this will toggle tracking
				trackingEnabled = !trackingEnabled;
				if (trackingEnabled == false) cout << "Tracking disabled." << endl;
				else cout << "Tracking enabled." << endl;
				break;
			case 100: //'d' has been pressed. this will debug mode
				debugMode = !debugMode;
				if (debugMode == false) cout << "Debug mode disabled." << endl;
				else cout << "Debug mode enabled." << endl;
				break;
			case 112: //'p' has been pressed. this will pause/resume the code.
				pause = !pause;
				if (pause == true) {
					cout << "Code paused, press 'p' again to resume" << endl;
					while (pause == true) {
						switch (waitKey()) {
						case 112:
							//change pause back to false
							pause = false;
							cout << "Code resumed." << endl;
							break;
						}
					}
				}
			}
		}
		capture.release();
	}


	return 0;

}

