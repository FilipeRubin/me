#pragma once
#include "win32-window-class.h"

class Win32Window
{
public:
	static void PollGeneralMessages();
	static unsigned int GetRunningWindowsCount();
	void Destroy();
	bool IsRunning() const;
	void PollWindowMessages();
	bool TryCreate();
private:
	static Win32WindowClass s_windowClass;
	static unsigned int s_runningWindowsCount;
	static void DecreaseCounter();
	static bool TryIncreaseCounter();
	bool TryCreateAndShow();
	void* m_hwnd;
};
