///////////////////////////////////////////////////////////////////////
// OpenCV tracking example.
// Written by darkpgmr (http://darkpgmr.tistory.com), 2013

#include <TCHAR.h>
#include <iostream>
#include <windows.h>
#include "tracker_opencv.h"

using namespace std;

struct CallbackParam
{
	Mat frame;
	Point pt1,pt2;
	Rect roi;
	bool drag;
	bool updated;
};

void onMouse( int event, int x, int y, int flags, void* param )
{
	CallbackParam *p = (CallbackParam *)param;

	if( event == EVENT_LBUTTONDOWN)
	{
		p->pt1.x = x;
		p->pt1.y = y;
		p->pt2 = p->pt1;
		p->drag = true;
	}
	if( event == EVENT_LBUTTONUP )
	{
		int w = x - p->pt1.x;
		int h = y - p->pt1.y;

		p->roi.x = p->pt1.x;
		p->roi.y = p->pt1.y;
		p->roi.width = w;
		p->roi.height = h;
		p->drag = false;

		if(w>=10 && h>=10)
		{
			p->updated = true;
		}
	}
	if( p->drag && event == EVENT_MOUSEMOVE )
	{
		if(p->pt2.x != x || p->pt2.y != y)
		{
			Mat img = p->frame.clone();
			p->pt2.x = x;
			p->pt2.y = y;
			rectangle(img, p->pt1, p->pt2, Scalar(0,255,0), 1);
			imshow("image", img);
		}
	}
}

void proc_video(VideoCapture *vc)
{
	// tracker 만들고 configure (알고리즘 선택)
	tracker_opencv tracker;
	tracker.configure();

	// frmae에 비디오
	Mat frame;
	*vc >> frame;
	imshow("image", frame);

	// Rect 그리기 위한 Struct 같음
	CallbackParam param;
	param.frame = frame;
	param.drag = false;
	param.updated = false;
    setMouseCallback("image", onMouse, &param);

	bool tracking = false;
	while(1)
	{
		// Drag가 True이면 (사각형이 있으면? 클릭이 한번되면)
		// image acquisition & target init
		if(param.drag)
		{
		    if( waitKey(10) == 27 ) break;		// ESC key
			continue;
		}
		// 지속적 업데이트 (마우스 클릭후 업 된 순간부터)
		if(param.updated)
		{
			Rect rc = param.roi;
			tracker.init(frame, rc);
			param.updated = false;
			tracking = true;
		}

		// 비디오는 계속 비면 break
		*vc >> frame;
		if(frame.empty()) break;

		// image processing
		if(tracking)
		{
			Rect rc;
			bool ok = tracker.run(frame, rc);
		}

		// image display
		imshow("image", frame);

		// user input
		char ch = waitKey(10);
		if( ch == 27 ) break;	// ESC Key (exit)
		else if(ch == 32 )	// SPACE Key (pause)
		{
			while((ch = waitKey(10)) != 32 && ch != 27);
			if(ch == 27) break;
		}
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	//select image source
	char data_src;
	cout << "1. camera input (640 x 480)\n"
		 << "2. camera input (320 x 240)\n"
		 << "3. video file input\n"
		 << endl
		 << "select video source[1-3]: ";
	cin >> data_src;

	VideoCapture *vc = NULL;
	if(data_src=='1')
	{
		//camera (vga)
		vc = new VideoCapture(0);
		if (!vc->isOpened())
		{
			cout << "can't open camera" << endl;
			return 0;
		}
		vc->set(cv::CAP_PROP_FRAME_WIDTH, 640);
		vc->set(cv::CAP_PROP_FRAME_HEIGHT, 480);
	}
	else if(data_src=='2')
	{
		//camera (qvga)
		vc = new VideoCapture(0);
		if (!vc->isOpened())
		{
			cout << "can't open camera" << endl;
			return 0;
		}
		vc->set(cv::CAP_PROP_FRAME_WIDTH, 320);
		vc->set(cv::CAP_PROP_FRAME_HEIGHT, 240);
	}
	//else if(data_src=='3')
	//{
	//	//video(avi)
	//	OPENFILENAME ofn;
	//	char szFile[MAX_PATH] = "";
	//	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	//	ofn.lStructSize = sizeof(OPENFILENAME);
	//	ofn.hwndOwner = NULL;
	//	ofn.lpstrFile = szFile;
	//	ofn.nMaxFile = sizeof(szFile);
	//	ofn.lpstrFilter = _T("Avi Files(*.avi)\0*.avi\0All Files (*.*)\0*.*\0");
	//	ofn.nFilterIndex = 1;
	//	ofn.lpstrFileTitle = NULL;
	//	ofn.nMaxFileTitle = 0;
	//	ofn.lpstrInitialDir = NULL;
	//	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	//	if(::GetOpenFileName(&ofn)==false) return 0;

	//	vc = new VideoCapture(ofn.lpstrFile);
	//	if (!vc->isOpened())
	//	{
	//		cout << "can't open video file" << endl;
	//		return 0;
	//	}
	//}

	// vc (동영상 캡처) == 즉, 동영상 있으면 proc_video로
	if(vc) proc_video(vc);
	if(vc) delete vc;

	destroyAllWindows();

	return 0;
}
