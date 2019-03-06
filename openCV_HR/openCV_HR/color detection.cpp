#include <opencv2/opencv.hpp>
#include <iostream>
#include "DataWriter.h"

using namespace cv;
using namespace std;

int main()
{

	CDataWriter DataWriter;

	int range_count = 0;

	Scalar red(0, 0, 255);
	//Scalar blue(255, 0, 0);
	//Scalar yellow(0, 255, 255);

	//Scalar magenta(255, 0, 255);


	Mat rgb_color = Mat(1, 1, CV_8UC3, red);
	Mat hsv_color;

	cvtColor(rgb_color, hsv_color, COLOR_BGR2HSV);


	int hue = (int)hsv_color.at<Vec3b>(0, 0)[0];
	int saturation = (int)hsv_color.at<Vec3b>(0, 0)[1];
	int value = (int)hsv_color.at<Vec3b>(0, 0)[2];


	cout << "hue = " << hue << endl;
	cout << "saturation = " << saturation << endl;
	cout << "value = " << value << endl;


	int low_hue = hue - 10;
	int high_hue = hue + 10;

	int low_hue1 = 0, low_hue2 = 0;
	int high_hue1 = 0, high_hue2 = 0;

	if (low_hue < 10) {
		range_count = 2;

		high_hue1 = 180;
		low_hue1 = low_hue + 180;
		high_hue2 = high_hue;
		low_hue2 = 0;
	}
	else if (high_hue > 170) {
		range_count = 2;

		high_hue1 = low_hue;
		low_hue1 = 180;
		high_hue2 = high_hue - 180;
		low_hue2 = 0;
	}
	else {
		range_count = 1;

		low_hue1 = low_hue;
		high_hue1 = high_hue;
	}


	cout << low_hue1 << "  " << high_hue1 << endl;
	cout << low_hue2 << "  " << high_hue2 << endl;


	//--- INITIALIZE VIDEOCAPTURE
	VideoCapture cap;

	//int deviceID = 0;					   // 0 = open default camera
	//int apiID = cv::CAP_ANY;      // 0 = autodetect default API

	//cap.open(deviceID + apiID);

	Mat img_frame, img_hsv;

	//---비디오로 할 때---
	cap.open("3.mp4");

	if (!cap.isOpened()) {
		cerr << "ERROR! Unable to open camera\n";
		return -1;
	}



	

	for (;;)
	{

		// wait for a new frame from camera and store it into 'frame'
		cap.read(img_frame);

		// check if we succeeded
		if (img_frame.empty()) {
			cerr << "ERROR! blank frame grabbed\n";
			break;
		}


		//HSV로 변환
		cvtColor(img_frame, img_hsv, COLOR_BGR2HSV);


		//지정한 HSV 범위를 이용하여 영상을 이진화
		Mat img_mask1, img_mask2;
		inRange(img_hsv, Scalar(low_hue1, 150, 50), Scalar(high_hue1, 255, 255), img_mask1);
		if (range_count == 2) {
			inRange(img_hsv, Scalar(low_hue2, 150, 50), Scalar(high_hue2, 255, 255), img_mask2);
			img_mask1 |= img_mask2;
		}


		//morphological opening 작은 점들을 제거 
		erode(img_mask1, img_mask1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(img_mask1, img_mask1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));


		//morphological closing 영역의 구멍 메우기 
		dilate(img_mask1, img_mask1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(img_mask1, img_mask1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));


		//라벨링 
		Mat img_labels, stats, centroids;
		int numOfLables = connectedComponentsWithStats(img_mask1, img_labels,
			stats, centroids, 8, CV_32S);

		
		int max = -1, idx = 0;

		for (int j = 1; j < numOfLables; ++j) {
			int area = stats.at<int>(j, CC_STAT_AREA); //가장 큰 영역을 얻어내서 max에 저장한다.
			if (max < area)
			{
				max = area;
				idx = j;
			}
		}


		int left = stats.at<int>(idx, CC_STAT_LEFT); 
		int top = stats.at<int>(idx, CC_STAT_TOP);
		int width = stats.at<int>(idx, CC_STAT_WIDTH);
		int height = stats.at<int>(idx, CC_STAT_HEIGHT);

		int right = left + width;
		int bottom = top + height;

		//--------------------------------------------------------------------
		int left_y = -1;

		for (int x = left - 20; x < left + 20; ++x)
			for (int y = top; y < bottom; ++y)
		{
			int b = img_frame.at<Vec3b>(y, x)[0];
			int g = img_frame.at<Vec3b>(y, x)[1];
			int r = img_frame.at<Vec3b>(y, x)[2];
					
			if ((r > 100) && (r*1.3 > g + b))  //원래 이진화된 영상에서 검사해야하지만 음 일단...빨강을 대강 찾음..
			{
				left_y = y;
				goto a;
			}
		}
		a:
		circle(img_frame, Point(left, left_y), 10, Scalar(0, 255, 0), 1);

		//--------------------------------------------------------------------
		int top_x = -1;

	
		for (int y = top; y < top + 20; ++y)
		{	
			for (int x = left; x < right; ++x)
			{
				int b = img_frame.at<Vec3b>(y, x)[0];
				int g = img_frame.at<Vec3b>(y, x)[1];
				int r = img_frame.at<Vec3b>(y, x)[2];

				if ((r > 100) && (r*1.3 > g + b)) 
				{
					top_x = x;
					goto b;
				}
			}
		}

		b:

		circle(img_frame, Point(top_x, top), 10, Scalar(0, 255, 0), 1);


		//--------------------------------------------------------------------

		int right_y = -1;
		for (int x = right - 30; x < right; ++x)
			for (int y = top; y < bottom; ++y)
			{
				int b = img_frame.at<Vec3b>(y, x)[0];
				int g = img_frame.at<Vec3b>(y, x)[1];
				int r = img_frame.at<Vec3b>(y, x)[2];

				if ((r > 100) && (r*1.3 > g + b))
				{
					right_y = y;

					goto c;
				}
			}

		c:
		circle(img_frame, Point(right, right_y), 10, Scalar(0, 255, 0), 1);


		//--------------------------------------------------------------------
		int bottom_x = -1;

		for (int y = bottom - 20; y < bottom; ++y)
			for (int x = left; x < right; ++x)
			{
				int b = img_frame.at<Vec3b>(y, x)[0];
				int g = img_frame.at<Vec3b>(y, x)[1];
				int r = img_frame.at<Vec3b>(y, x)[2];

				if ((r > 100) && (r*1.3 > g + b))
				{
					bottom_x = x;
					break;
				}
			}

		circle(img_frame, Point(bottom_x, bottom), 10, Scalar(0, 255, 0), 1);


		//Mat ROI = stats.adjustROI(top, bottom, left, right);


		int x = centroids.at<double>(idx, 0); 
		int y = centroids.at<double>(idx, 1);
		//cout << "center: " << x << "," << y << endl;
		//
		//cout << left << "~" << right << endl;
		//cout << top << "~" << bottom << endl;
		//cout << width << endl;
		//cout << height << endl;
		//cout << "-------------------\n";


		//
		line(img_frame, Point(left, left_y), Point(right, right_y), Scalar(0, 255, 255), 10);
		line(img_frame, Point(top_x, top), Point(bottom_x, bottom), Scalar(0, 255, 255), 10);
		circle(img_frame, Point(x, y), 5, Scalar(255, 0, 0), 1);


		DataWriter.Append(cap.get(CAP_PROP_POS_MSEC),x,y);

		imshow("이진화 영상", img_mask1);
		imshow("원본 영상", img_frame);


		if (waitKey(5) >= 0)
			break;
	}
	DataWriter.WriteDataToFile("data");

	return 0;
}
