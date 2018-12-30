#include "stdafx.h"
#include "TrackerModule.h"


CTrackerModule::CTrackerModule()
{
	

}


CTrackerModule::~CTrackerModule()
{
}

bool CTrackerModule::Initialize(cv::VideoCapture* _video)
{

	// Read first frame 
	bool ok = _video->read(m_mat);

	m_bbox = cv::Rect2d(200, 120, 120, 40);

	// Uncomment the line below to select a different bounding box 
	// bbox = selectROI(frame, false); 
	// Display bounding box. 
	rectangle(m_mat, m_bbox, cv::Scalar(255, 0, 0), 2, 1);

	imshow("Tracking", m_mat);
	tracker->create();
	return true;
}

bool CTrackerModule::TrackObject()
{


	// Start timer
	double timer = (double)cv::getTickCount();

	// Update the tracking result
	bool ok = tracker->update(m_mat, m_bbox);

	// Calculate Frames per second (FPS)
	float fps = cv::getTickFrequency() / ((double)cv::getTickCount() - timer);

	if (ok)
	{
		// Tracking success : Draw the tracked object
		rectangle(m_mat, m_bbox, cv::Scalar(255, 0, 0), 2, 1);
	}
	else
	{
		// Tracking failure detected.
		putText(m_mat, "Tracking failure detected", cv::Point(100, 80), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(0, 0, 255), 2);
	}

	// Display tracker type on frame
	putText(m_mat, " Tracker", cv::Point(100, 20), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(50, 170, 50), 2);

	// Display FPS on frame
	//putText(frame, "FPS : " + SSTR(int(fps)), Point(100, 50), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50, 170, 50), 2);

	// Display frame.
	cv::imshow("Tracking", m_mat);



	return true;
}


