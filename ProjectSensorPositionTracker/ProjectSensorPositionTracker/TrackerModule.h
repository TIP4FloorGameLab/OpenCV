#pragma once
class CTrackerModule
{
public:
	CTrackerModule();
	~CTrackerModule();
	bool Initialize(cv::VideoCapture& _video);
	bool TrackObject(const Mat& img, Mat& mask, Mat& dst, Mat& result);
private:
	cv::Mat m_mat;
	
	Ptr<BackgroundSubtractorMOG2> bgsubtractor;
	bool update_bg_model;


};

