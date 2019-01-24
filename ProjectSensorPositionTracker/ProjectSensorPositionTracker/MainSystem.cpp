#include "stdafx.h"
#include "BufferManager.h"
#include "CaptureSystem.h"
#include "TrackerModule.h"
#include "MainSystem.h"


CMainSystem::CMainSystem()
{
	m_CaptureSystem = new CCaptureSystem(CAPTUREMODE::CAPTUREMODE_CAM);
	m_BufferManager = new CBufferManager();
	m_TrackerModule = new CTrackerModule();
}


CMainSystem::~CMainSystem()
{
}

void CMainSystem::GetCaptureData()
{
	m_CaptureSystem->GetCaptureData() >> m_BufferManager->GetMat(MATIDX::MAT_CAPTURE);
	m_TrackerModule->Initialize(m_CaptureSystem->GetCaptureData());

	
}

void CMainSystem::CalculateBuffer()
{
	m_CaptureSystem->GetCaptureData() >> m_BufferManager->GetMat(MATIDX::MAT_CAPTURE);
	
	m_BufferManager->GetMat(MATIDX::MAT_TEMP_FRAME) = m_BufferManager->GetMat(MATIDX::MAT_CAPTURE);

	//Mat& TempF =	 m_BufferManager->GetMat(MATIDX::MAT_TEMP_FRAME);
	//Mat& BKF =			m_BufferManager->GetMat(MATIDX::MAT_BACKGROUNDMASK);
	//Mat& OUTF =			m_BufferManager->GetMat(MATIDX::MAT_OUT_FRAME);

	m_TrackerModule->TrackObject(
	m_BufferManager->GetMat(MATIDX::MAT_TEMP_FRAME),
	m_BufferManager->GetMat(MATIDX::MAT_BACKGROUNDMASK),
	m_BufferManager->GetMat(MATIDX::MAT_OUT_FRAME),
	m_BufferManager->GetMat(MATIDX::MAT_RESULT)
	);

}

void CMainSystem::RenderBuffer()
{
	cv::imshow("Captured Image", m_BufferManager->GetMat(MATIDX::MAT_CAPTURE));
	cv::imshow("BackGroundMask", m_BufferManager->GetMat(MATIDX::MAT_BACKGROUNDMASK));
	cv::imshow("Output", m_BufferManager->GetMat(MATIDX::MAT_OUT_FRAME));
	cv::imshow("Track", m_BufferManager->GetMat(MATIDX::MAT_RESULT));
}

void CMainSystem::FrameAdvance()
{
	cv::Mat& frame = m_BufferManager->GetMat(MATIDX::MAT_CAPTURE);
	while (m_CaptureSystem->GetCaptureData().read(frame))
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
