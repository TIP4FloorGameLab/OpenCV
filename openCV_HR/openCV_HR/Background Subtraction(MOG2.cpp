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
//Ptr< BackgroundSubtractor> pMOG2;
//
//Mat fgMaskMOG2;
//
//int main()
//{
//	Mat img_input;//current frame
//	Mat img_binary;
//
//	//pMOG2 = createBackgroundSubtractorMOG2(100, 500, 0);
//	adaptiveThreshold(fgMaskMOG2, img_binary, 10, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 101, 50);
//
//
//
//	//--- INITIALIZE VIDEOCAPTURE
//	VideoCapture cap;
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
//		cap.read(img_input);
//
//
//		pMOG2->apply(img_input, fgMaskMOG2);
//
//
//
//		imshow("Origin", img_input);
//		imshow("MOG2", fgMaskMOG2);
//
//
//		if (waitKey(5) == 27) //ESC 
//			break;
//	}
//
//	return 0;
//}