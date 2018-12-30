#include "stdafx.h"
#include "BufferManager.h"
#include "CaptureSystem.h"
#include "TrackerModule.h"
#include "MainSystem.h"


CMainSystem::CMainSystem()
{
	m_CaptureSystem = std::make_shared<CCaptureSystem>(CAPTUREMODE::CAPTUREMODE_CAM);
	m_BufferManager = std::make_shared<CBufferManager>();
	m_TrackerModule = std::make_shared<CTrackerModule>();
}


CMainSystem::~CMainSystem()
{
}

void CMainSystem::GetCaptureData()
{

	//// 스트리밍 되고 있는 폭과 높이 값을 가져온다.
	//int width = (int)cvGetCaptureProperty(m_CaptureSystem->GetCaptureData(), CV_CAP_PROP_FRAME_WIDTH);
	//int height = (int)cvGetCaptureProperty(m_CaptureSystem->GetCaptureData(), CV_CAP_PROP_FRAME_HEIGHT);
	//CvSize size = cvSize(width, height);
	//
	//IplImage* CaptureFrame = m_BufferManager->GetBuffer(BufferIDX::BUF_CAPTURE);
	//InitFrameBuffer(CaptureFrame, size);
	//CaptureFrame = cvQueryFrame(m_CaptureSystem->GetCaptureData());

	cv::Mat* frame = m_BufferManager.get()->GetMat(MATIDX::MAT_CAPTURE);
	bool ok = m_CaptureSystem.get()->GetCaptureData()->read(*frame);
	m_TrackerModule->Initialize(m_CaptureSystem.get()->GetCaptureData());

	
}

void CMainSystem::CalculateBuffer()
{

	m_TrackerModule->TrackObject();

}

void CMainSystem::RenderBuffer()
{
	if(m_BufferManager->GetMat(MATIDX::MAT_CAPTURE))
		cv::imshow("Captured Image", *m_BufferManager->GetMat(MATIDX::MAT_CAPTURE));
}

void CMainSystem::FrameAdvance()
{
	cv::Mat* frame = m_BufferManager.get()->GetMat(MATIDX::MAT_CAPTURE);
	while (m_CaptureSystem->GetCaptureData()->read(*frame))
	{
		CalculateBuffer();
		RenderBuffer();

		int k = cv::waitKey(1);
		if (k == 27)
		{
			break;
		}

	}
	
}
