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
		// 0��° ����� ī�޶�� ���� ��Ʈ���� ���´�.
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
