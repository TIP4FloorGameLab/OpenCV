#pragma once

enum class CAPTUREMODE {
	CAPTUREMODE_CAM = 0,
	CAPTUREMODE_MEDIA
};

class CCaptureSystem
{
public:
	CCaptureSystem();
	CCaptureSystem(CAPTUREMODE _capturemode);
	~CCaptureSystem();
	cv::VideoCapture* GetCaptureData() { return m_MainCapture; }
private:
	cv::VideoCapture* m_MainCapture;
	
};

