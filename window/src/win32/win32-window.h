#pragma once
#include "win32-window-class.h"
#include "window-parameters.h"

class Win32Window
{
public:
	static void PollGeneralMessages();
	static unsigned int GetRunningWindowsCount();
	void Destroy();
	const std::string& GetLastErrorInformation() const;
	bool IsRunning() const;
	void PollWindowMessages();
	bool TryCreate(const WindowParameters& parameters);
private:
	static Win32WindowClass s_windowClass;
	static unsigned int s_runningWindowsCount;
	static void DecreaseCounter();
	static bool TryIncreaseCounter();
	bool TryCreateAndShow(const WindowParameters& parameters);
	void* m_hwnd;
	std::string m_lastErrorInfo;
};
