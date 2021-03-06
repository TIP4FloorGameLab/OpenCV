#include "stdafx.h"
#include "preprocess.hpp"



int main()
{
	Mat frame1, frame2;

	VideoCapture capture;


	while (1) {


		capture.open("../video/bouncingBall.avi");

		if (!capture.isOpened()) {
			cout << "ERROR ACQUIRING VIDEO FEED\n";
			getchar();
			return -1;
		}

		//check if the video has reach its last frame.
		//we add '-1' because we are reading two frames from the video at a time.
		//if this is not included, we get a memory error!
		while (capture.get(CV_CAP_PROP_POS_FRAMES) < capture.get(CV_CAP_PROP_FRAME_COUNT) - 1) {

			//read first frame
			capture.read(frame1);
			imshow("Frame1", frame1);



			CV_Assert(frame1.data);

			Mat th_img = preprocessing(frame1);
			vector<RotatedRect> rects = find_rects(th_img);
			RotatedRect rect = rects[0];
			Rect bound_rect = rect.boundingRect();

			rectangle(frame1, bound_rect, Scalar(0, 255, 0), 1);
			cout << rect.center << endl;

			/*for (int i = 0; i < rects.size(); ++i)
			{
			float radius = rects[i].angle;
			circle(image, rects[i].center, radius, Scalar(0, 255, 0), 2);
			cout << radius << endl;
			}*/
			imshow("전처리영상", th_img);
			imshow("사각형영상", frame1);

			waitKey();

			capture.release();

		}
	}

	return 0;


	// for문 돌리기 - 사진으로 추출
	///////////////////////////////////////////////////////////////////////
	//int num = 0;

	//for (num; num < 40; ++num)
	//{
	//	
	//	String fname = format("../image/capture/%d.png", num);
	//	
	//	Mat image = imread(fname, 1);
	//	
	//	CV_Assert(image.data);

	//	Mat th_img = preprocessing(image);
	//	vector<RotatedRect> rects = find_rects(th_img);
	//	RotatedRect rect = rects[0];
	//	Rect bound_rect = rect.boundingRect();

	//	rectangle(image, bound_rect, Scalar(0, 255, 0), 1);
	//	cout << rect.center << endl;

	//	/*for (int i = 0; i < rects.size(); ++i)
	//	{
	//	float radius = rects[i].angle;
	//	circle(image, rects[i].center, radius, Scalar(0, 255, 0), 2);
	//	cout << radius << endl;
	//	}*/
	//	imshow("전처리영상", th_img);
	//	imshow("사각형영상", image);
	//	waitKey();

	//}		
	//

	//return 0;


	//VideoCapture capture;

	//capture.open("../video/video.mp4");
	//CV_Assert(capture.isOpened());

	//double frame_rate = capture.get(CV_CAP_PROP_FPS);
	//int delay = 1000 / frame_rate;
	//int frame_cnt = 0;
	//Mat frame;

	//while(capture.read(frame))
	//{
	//	if (waitKey(delay) >= 0)	break;


	//	//put_string(frame, "frame_cnt", Point(20, 50), frame_cnt);
	//	imshow("동영상 파일 읽기", frame);
	//}


	//return 0;

}
