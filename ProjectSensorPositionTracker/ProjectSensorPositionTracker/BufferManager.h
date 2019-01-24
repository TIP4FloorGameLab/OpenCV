#pragma once

class CBufferManager
{
public:
	CBufferManager();
	~CBufferManager();
	void AppendBuffer();
	cv::Mat& GetMat(MATIDX _idx);
private:
	std::map<MATIDX, cv::Mat> m_Mats;
};

