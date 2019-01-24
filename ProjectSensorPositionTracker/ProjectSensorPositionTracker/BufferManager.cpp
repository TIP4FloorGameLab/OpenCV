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
	//for(int i =0; i< static_cast<int>(MATIDX::COUNT) ;++i)
	//	m_Mats.emplace(std::make_pair(static_cast<MATIDX>(i), new cv::Mat()));
	m_Mats.insert(std::make_pair(MATIDX::MAT_TEMP_FRAME, cv::Mat()));
	m_Mats.insert(std::make_pair(MATIDX::MAT_OUT_FRAME, cv::Mat()));
	m_Mats.insert(std::make_pair(MATIDX::MAT_CAPTURE, cv::Mat()));
	m_Mats.insert(std::make_pair(MATIDX::MAT_BACKGROUNDMASK, cv::Mat()));
	m_Mats.insert(std::make_pair(MATIDX::MAT_RESULT, cv::Mat()));
}



cv::Mat& CBufferManager::GetMat(MATIDX _idx)
{
	return m_Mats[_idx];
}




