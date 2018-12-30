// ProjectSensorPositionTracker.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "MainSystem.h"
#include "ProjectSensorPositionTracker.h"

void main()
{
	CMainSystem* mainSystem = new CMainSystem();

	mainSystem->GetCaptureData();
	mainSystem->FrameAdvance();

}