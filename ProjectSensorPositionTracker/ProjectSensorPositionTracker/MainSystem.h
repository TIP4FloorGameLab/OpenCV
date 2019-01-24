#pragma once

class CCaptureSystem;
class CBufferManager;
class CTrackerModule;

class CMainSystem
{
public:
	CMainSystem();
	~CMainSystem();

	void GetCaptureData();
	void CalculateBuffer();
	void RenderBuffer();
	void FrameAdvance();
private:
	CCaptureSystem* m_CaptureSystem;
	CBufferManager* m_BufferManager;
	CTrackerModule* m_TrackerModule;
};

