#include <opencv2\opencv.hpp>
using namespace cv;

Mat preprocessing(Mat img)
{
	Mat gray, th_img;
	cvtColor(img, gray, CV_BGR2GRAY);
	GaussianBlur(gray, gray, Size(7, 7), 1, 1);

	threshold(gray, th_img, 130, 255, THRESH_BINARY | THRESH_OTSU);
	morphologyEx(th_img, th_img, MORPH_OPEN, Mat(), Point(-1, -1), 1);

	return th_img;
}

vector<RotatedRect>	find_rects(Mat img)
{
	vector<vector<Point>> contours;  //¿Ü°û¼± °ËÃâ

	findContours(img.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	vector<RotatedRect> rects;
	for (int i = 0; i < (int)contours.size(); ++i)
	{
		RotatedRect min_rect = minAreaRect(contours[i]);
		min_rect.angle = (min_rect.size.width + min_rect.size.height) / 4.0f;
	
		if (min_rect.angle > 25) rects.push_back(min_rect);
	}

	return rects;
}