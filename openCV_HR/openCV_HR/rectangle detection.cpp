//#include "opencv2/opencv.hpp" 
//#include <iostream>  
//#include <string> 
//
//
//using namespace cv;
//using namespace std;
//
//
//
//void setLabel(Mat& image, vector<Point> contour, Point* center)
//{
//	int fontface = FONT_HERSHEY_SIMPLEX;
//	double scale = 0.4;
//	int thickness = 1;
//
//	stringstream ss;
//	ss << center->x << ", " << center->y;
//	string tmp = ss.str();
//
//	putText(image, tmp, Point(center->x - 20, center->y - 20), fontface, scale, CV_RGB(1, 0, 0), thickness, 8);
//}
//
//int main(int, char**)
//{
//	Mat img_input, img_result, img_gray;
//
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
//
//
//
//	for (;;)
//	{
//
//		cap.read(img_input);
//
//		if (img_input.empty()) {
//			cerr << "ERROR! blank frame grabbed\n";
//			break;
//		}
//
//
//		Mat img_temp;
//		//그레이스케일 이미지로 변환  
//		cvtColor(img_input, img_temp, COLOR_BGR2HSV);
//		GaussianBlur(img_temp, img_temp, Size(5, 5), 0, 0);
//
//
//		//이진화 이미지로 변환
//		adaptiveThreshold(img_temp, img_gray,
//			255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV,
//			201, 7);                                  // <-----------------------   parameter 1,  2
//
//
//		//contour를 찾는다.
//		vector<vector<Point> > contours;
//		findContours(img_gray, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
//
//		//contour를 근사화한다.
//		vector<Point2f> approx;
//		img_result = img_input.clone();
//
//		for (size_t i = 0; i < contours.size(); i++)
//		{
//			approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);
//
//			if (fabs(contourArea(Mat(approx))) > 1000 && fabs(contourArea(Mat(approx))) < 10000)  // <-----------------------   parameter 3
//			{
//
//				int size = approx.size();
//				const char *name[] = { "none", "none", "none", "triangle", "quadrangle", "pentagon", "hexagon", "heptagon", "octagon", "nonagon", "decagon" };
//
//				switch (size) {
//
//				case 4: //사각형
//				
//					if (isContourConvex(Mat(approx))) { // convex 인지 검사 
//
//						
//					//Contour를 근사화한 직선을 그린다.
//						line(img_result, approx[0], approx[approx.size() - 1], Scalar(0, 255, 0), 3);
//
//						for (int k = 0; k < size - 1; k++)
//							line(img_result, approx[k], approx[k + 1], Scalar(0, 255, 0), 3);
//
//						for (int k = 0; k < size; k++)
//							circle(img_result, approx[k], 3, Scalar(0, 0, 255)); //꼭짓점
//
//
//						int cx = int((approx[1].x + approx[3].x) / 2);
//						int cy = int((approx[1].y + approx[3].y) / 2);
//
//						circle(img_result,Point2f(cx, cy), 4, (0, 0, 255), -1); //직사각형 무게 중심
//
//						
//						//검출된 도형에 대한 라벨을 출력한다.
//						setLabel(img_result, contours[i], &Point(cx, cy));
//
//						int rotatez = (approx[3].y - approx[1].y) / (approx[3].x - approx[1].x);
//						//cout << "x" << "y" << "z"<<	z << endl;
//					}
//
//					break;
//
//
//
//				default:
//
//					break;
//				}
//
//			}
//		}
//
//
//		imshow("result", img_result);
//		imshow("binary", img_gray);
//
//		if (waitKey(5) == 27) //ESC 키 누를 때 까지 대기
//			break;
//
//	}
//
//	return 0;
//}
