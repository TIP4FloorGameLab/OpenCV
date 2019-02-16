//#include "opencv2/opencv.hpp" 
//#include "opencv2/bgsegm.hpp"
//#include "opencv2/cudabgsegm.hpp"
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgcodecs.hpp>  
//#include <opencv2/imgproc.hpp> 
//
//#include <iostream>  
//#include <time.h>
//
//using namespace std;
//using namespace cv;
////using namespace traits;
//
//Ptr< BackgroundSubtractorMOG2> pMOG2;
//
//Mat resizeMOG, resizeOrg, resizeLabel;
//
//static void refineSegments(const Mat& img, Mat& mask, Mat& dst)
//{
//	int niters = 3;
//
//	vector<vector<Point> > contours;
//	vector<Vec4i> hierarchy;
//
//	Mat temp;
//
//	dilate(mask, temp, Mat(), Point(-1, -1), niters);
//	erode(temp, temp, Mat(), Point(-1, -1), niters * 2);
//	dilate(temp, temp, Mat(), Point(-1, -1), niters);
//
//	findContours(temp, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
//
//	dst = Mat::zeros(img.size(), CV_8UC3);
//
//	if (contours.size() == 0)
//		return;
//
//	// iterate through all the top-level contours,
//	// draw each connected component with its own random color
//	int idx = 0, largestComp = 0;
//	double maxArea = 0;
//
//	for (; idx >= 0; idx = hierarchy[idx][0])
//	{
//		const vector<Point>& c = contours[idx];
//		double area = fabs(contourArea(Mat(c)));
//		if (area > maxArea)
//		{
//			maxArea = area;
//			largestComp = idx;
//		}
//	}
//	Scalar color(0, 0, 255);
//	drawContours(dst, contours, largestComp, color, FILLED, LINE_8, hierarchy);
//}
//
//int main()
//{
//	Mat frame;//current frame
//	Mat out_frame;
//	Mat fgMaskMOG2;
//
//	pMOG2 = createBackgroundSubtractorMOG2();
//	pMOG2->setVarThreshold(10);
//
//
//	Mat img_color, img_gray;
//
//	
//	//--- INITIALIZE VIDEOCAPTURE
//	VideoCapture cap;
//	bool update_bg_model = true;
//
//	int deviceID = 0;					   // 0 = open default camera
//	int apiID = cv::CAP_ANY;      // 0 = autodetect default API
//
//	cap.open(deviceID + apiID);
//
//	if (!cap.isOpened()) {
//		cerr << "ERROR! Unable to open camera\n";
//		return -1;
//	}
//
//
//	while (true) {
//
//		cap.read(frame);
//
//
//		pMOG2->apply(frame, fgMaskMOG2, update_bg_model ? -1 : 0);
//		refineSegments(frame, fgMaskMOG2, out_frame);
//
//		
//		adaptiveThreshold(fgMaskMOG2, img_gray, 10, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 101, 50);
//
//		threshold(fgMaskMOG2, img_gray, 200, 255, THRESH_BINARY);
//		cvtColor(fgMaskMOG2, img_color, COLOR_GRAY2BGR);
//
//
//		Mat img_labels, stats, centroids;
//		int numOfLables = connectedComponentsWithStats(img_gray, img_labels,
//			stats, centroids, 8, CV_32S);
//
//
//
//
//		////라벨링 된 이미지에 각각 직사각형으로 둘러싸기 
//		for (int j = 1; j < numOfLables; j++) {
//			int area = stats.at<int>(j, CC_STAT_AREA);
//	
//			int left = stats.at<int>(j, CC_STAT_LEFT);
//			int top = stats.at<int>(j, CC_STAT_TOP);
//			int width = stats.at<int>(j, CC_STAT_WIDTH);
//			int height = stats.at<int>(j, CC_STAT_HEIGHT);
//
//			if (1000 < area && area < 2000)
//			{
//				circle(frame, Point((left + width * 0.5), (top + height * 0.5)), (0, 255, 0), 20);
//				rectangle(img_color, Point(left, top), Point(left + width, top + height),
//					Scalar(0, 0, 255), 1);
//			}
//		
//			/*putText(img_color, to_string(j), Point(left + 20, top + 20),
//				FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0), 2);*/
//		}
//
//		//resize(fgMaskMOG2, resizeMOG, Size(frame.size().width / 2, frame.size().height / 2));
//		//resize(frame, resizeOrg, Size(frame.size().width / 2, frame.size().height / 2));
//		//resize(img_color, resizeLabel, Size(frame.size().width , frame.size().height ));
//
//
//		imshow("Origin", frame);
//		imshow("segmented", out_frame);
//		//imshow("Label", resizeLabel);
//
//		if (waitKey(5) == 27) //ESC 
//			break;
//	}
//
//	return 0;
//}