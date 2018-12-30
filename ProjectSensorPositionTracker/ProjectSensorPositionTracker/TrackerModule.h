#pragma once
class CTrackerModule
{
public:
	CTrackerModule();
	~CTrackerModule();
	bool Initialize(cv::VideoCapture* _video);
	bool TrackObject();
private:
	cv::Ptr<cv::TrackerKCF> tracker;
	cv::Mat m_mat;
	// Define initial bounding box 
	cv::Rect2d m_bbox;
};

