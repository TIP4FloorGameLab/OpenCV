// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 특정 포함 파일이 들어 있는
// 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


#include <opencv2/opencv.hpp>
#include <opencv2/video/tracking.hpp>
#include <tracking/include/opencv2/tracking/tracker.hpp>
#include <opencv2/core/ocl.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui.hpp>

#ifdef _DEBUG
#pragma comment (lib, "opencv_world400d.lib")
#pragma comment (lib, "opencv_tracking400d.lib")
#else
#pragma comment (lib, "opencv_world400.lib")
#pragma comment (lib, "opencv_tracking400.lib")
#endif

// 여기서 프로그램에 필요한 추가 헤더를 참조합니다.

//using namespace cv;

enum class IPLIMAGEIDX
{
	IPLI_CAPTURE
};

enum class MATIDX
{
	MAT_CAPTURE
};