#include "stdafx.h"
#include "TrackerModule.h"


CTrackerModule::CTrackerModule()
{
	

}


CTrackerModule::~CTrackerModule()
{
}

bool CTrackerModule::Initialize(cv::VideoCapture& _video)
{

	// Read first frame 
	bool ok = _video.read(m_mat);
	bgsubtractor = createBackgroundSubtractorMOG2();
	bgsubtractor->setVarThreshold(10);
	update_bg_model = true;

	return true;
}

bool CTrackerModule::TrackObject(const Mat& img, Mat& mask, Mat& dst, Mat& result)
{
	bgsubtractor->apply(img, mask, update_bg_model ? -1 : 0);

	int niters = 3;

	std::vector<std::vector<Point> > contours;
	std::vector<Vec4i> hierarchy;

	Mat temp;

	dilate(mask, temp, Mat(), Point(-1, -1), niters);
	erode(temp, temp, Mat(), Point(-1, -1), niters * 2);
	dilate(temp, temp, Mat(), Point(-1, -1), niters);

	findContours(temp, contours, hierarchy, RETR_LIST, CHAIN_APPROX_SIMPLE);

	dst = Mat::zeros(img.size(), CV_8UC3);

	if (contours.size() == 0)
		return false;

	// iterate through all the top-level contours,
	// draw each connected component with its own random color
	int idx = 0, largestComp = 0;
	double maxArea = 0;

	for (; idx >= 0; idx = hierarchy[idx][0])
	{
		const std::vector<Point>& c = contours[idx];
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
	std::vector<Point2f> approx;
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

	result = img_result;
}


