#include "stdafx.h"
#include "CaptureSystem.h"


CCaptureSystem::CCaptureSystem()
{
}

CCaptureSystem::CCaptureSystem(CAPTUREMODE _capturemode)
{
	switch (_capturemode)
	{
	case CAPTUREMODE::CAPTUREMODE_CAM:
		// 0번째 연결된 카메라로 부터 컨트롤을 얻어온다.
		m_MainCapture = cv::VideoCapture(0);
		break;
	case CAPTUREMODE::CAPTUREMODE_MEDIA:
		m_MainCapture = cv::VideoCapture("VIDEO\\Fire_VIDEO\\fire.mp4");
		break;
	}
	
}


CCaptureSystem::~CCaptureSystem()
{
}
