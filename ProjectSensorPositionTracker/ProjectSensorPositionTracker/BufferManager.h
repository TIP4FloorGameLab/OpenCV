#pragma once

class CBufferManager
{
public:
	CBufferManager();
	~CBufferManager();
	void AppendBuffer();
	IplImage* GetIplImage(IPLIMAGEIDX _idx);
	cv::Mat* GetMat(MATIDX _idx);
	void InitIplImage(IplImage* buffer, CvSize size);
	void ReleaseIplImage(IplImage* buffer);
private:
	std::map<IPLIMAGEIDX,IplImage *> m_IplImages;
	std::map<MATIDX, cv::Mat *> m_Mats;
};

