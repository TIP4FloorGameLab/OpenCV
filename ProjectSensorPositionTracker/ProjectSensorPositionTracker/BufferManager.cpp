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
	// ���� ( ũ�� , �� �̹����� ǥ���ϴ� ��Ʈ�� ũ�� , �̹����� ǥ���ϴ� ä���� ���� , ä��
	// IPL_DEPTH_8U :: ���� �����͸� 0~255 ������ ������ ǥ���ϰ���
	// ä�� ���� 1 �� ��� :: ȸ���� �̹���
	buffer = cvCreateImage(size, IPL_DEPTH_8U, 1);
	// ��� ���Ҹ� 0���� �����Ѵ�.
	cvZero(buffer);

}

void CBufferManager::ReleaseIplImage(IplImage * buffer)
{
}



