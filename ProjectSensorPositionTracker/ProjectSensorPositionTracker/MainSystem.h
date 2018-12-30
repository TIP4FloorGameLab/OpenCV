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
	std::shared_ptr<CCaptureSystem> m_CaptureSystem;
	std::shared_ptr<CBufferManager> m_BufferManager;
	std::shared_ptr<CTrackerModule> m_TrackerModule;
};

