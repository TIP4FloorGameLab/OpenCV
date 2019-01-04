#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/video/background_segm.hpp"
#include <stdio.h>
#include <string>

using namespace std;
using namespace cv;

void setLabel(Mat& image, vector<Point> contour, Point* center)
{
	int fontface = FONT_HERSHEY_SIMPLEX;
	double scale = 0.4;
	int thickness = 1;

	stringstream ss;
	ss << center->x << ", " << center->y;
	string tmp = ss.str();

	putText(image, tmp, Point(center->x - 20, center->y - 20), fontface, scale, CV_RGB(1, 0, 0), thickness, 8);
}

static void refineSegments(const Mat& img, Mat& mask, Mat& dst)
{
	int niters = 3;

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	Mat temp;

	dilate(mask, temp, Mat(), Point(-1, -1), niters);
	erode(temp, temp, Mat(), Point(-1, -1), niters * 2);
	dilate(temp, temp, Mat(), Point(-1, -1), niters);

	findContours(temp, contours, hierarchy, RETR_LIST, CHAIN_APPROX_SIMPLE);

	dst = Mat::zeros(img.size(), CV_8UC3);

	if (contours.size() == 0)
		return;

	// iterate through all the top-level contours,
	// draw each connected component with its own random color
	int idx = 0, largestComp = 0;
	double maxArea = 0;

	for (; idx >= 0; idx = hierarchy[idx][0])
	{
		const vector<Point>& c = contours[idx];
		double area = fabs(contourArea(Mat(c)));
		if (area > maxArea)
		{
			maxArea = area;
			largestComp = idx;
		}
	}
	Scalar color(0, 0, 255);
	

	drawContours(dst, contours, largestComp, color, FILLED, LINE_8, hierarchy);




	//contour를 근사화한다.
	vector<Point2f> approx;
	Mat img_result = img.clone();

	for (size_t i = 0; i < contours.size(); i++)
	{
		approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);

		if (fabs(contourArea(Mat(approx))) > 1000 && fabs(contourArea(Mat(approx))) < 10000)  // <-----------------------   parameter 3
		{

			int size = approx.size();
			const char *name[] = { "none", "none", "none", "triangle", "quadrangle", "pentagon", "hexagon", "heptagon", "octagon", "nonagon", "decagon" };

			switch (size) {

			case 4: //사각형

				if (isContourConvex(Mat(approx))) { // convex 인지 검사 


				//Contour를 근사화한 직선을 그린다.
					line(img_result, approx[0], approx[approx.size() - 1], Scalar(0, 255, 0), 3);

					for (int k = 0; k < size - 1; k++)
						line(img_result, approx[k], approx[k + 1], Scalar(0, 255, 0), 3);

					for (int k = 0; k < size; k++)
						circle(img_result, approx[k], 3, Scalar(0, 0, 255)); //꼭짓점


					int cx = int((approx[1].x + approx[3].x) / 2);
					int cy = int((approx[1].y + approx[3].y) / 2);

					circle(img_result, Point2f(cx, cy), 4, (0, 0, 255), -1); //직사각형 무게 중심


					//검출된 도형에 대한 라벨을 출력한다.
					setLabel(img_result, contours[i], &Point(cx, cy));
				}

				break;



			deafult:

				break;
			}

		}
	}
	imshow("result", img_result);

}


int main(int argc, char** argv)
{
	VideoCapture cap;
	bool update_bg_model = true;

	CommandLineParser parser(argc, argv, "{help h||}{@input||}");
	
	string input = parser.get<std::string>("@input");
	if (input.empty())
		cap.open(0);
	else
		cap.open(samples::findFileOrKeep(input));

	if (!cap.isOpened())
	{
		printf("\nCan not open camera or video file\n");
		return -1;
	}

	Mat tmp_frame, bgmask, out_frame;

	cap >> tmp_frame;
	if (tmp_frame.empty())
	{
		printf("can not read data from the video source\n");
		return -1;
	}

	namedWindow("video", 1);
	namedWindow("segmented", 1);

	Ptr<BackgroundSubtractorMOG2> bgsubtractor = createBackgroundSubtractorMOG2();
	bgsubtractor->setVarThreshold(10);

	for (;;)
	{
		cap >> tmp_frame;
		if (tmp_frame.empty())
			break;
		bgsubtractor->apply(tmp_frame, bgmask, update_bg_model ? -1 : 0);
		refineSegments(tmp_frame, bgmask, out_frame);

		imshow("video", tmp_frame);
		imshow("segmented", out_frame);

		char keycode = (char)waitKey(30);
		if (keycode == 27)
			break;
		if (keycode == ' ')
		{
			update_bg_model = !update_bg_model;
			printf("Learn background is in state = %d\n", update_bg_model);
		}
	}

	return 0;
}
