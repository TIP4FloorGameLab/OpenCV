#include "stdafx.h"
#include "BufferManager.h"


CBufferManager::CBufferManager()
{
	AppendBuffer();
}


CBufferManager::~CBufferManager()
{
}

void CBufferManager::AppendBuffer()
{
	m_IplImages.emplace(std::make_pair(IPLIMAGEIDX::IPLI_CAPTURE, new IplImage()));
	m_Mats.emplace(std::make_pair(MATIDX::MAT_CAPTURE, new cv::Mat()));
}

IplImage * CBufferManager::GetIplImage(IPLIMAGEIDX _idx)
{
	return m_IplImages[_idx];
}

cv::Mat * CBufferManager::GetMat(MATIDX _idx)
{
	return m_Mats[_idx];
}

void CBufferManager::InitIplImage(IplImage * buffer, CvSize size)
{
	// cvCreateImage
	// 인자 ( 크기 , 한 이미지를 표현하는 비트의 크기 , 이미지를 표현하는 채널의 갯수 , 채널
	// IPL_DEPTH_8U :: 색상 데이터를 0~255 사이의 정수로 표현하겠음
	// 채널 값이 1 일 경우 :: 회색조 이미지
	buffer = cvCreateImage(size, IPL_DEPTH_8U, 1);
	// 모든 원소를 0으로 설정한다.
	cvZero(buffer);

}

void CBufferManager::ReleaseIplImage(IplImage * buffer)
{
}



