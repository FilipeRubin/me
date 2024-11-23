#pragma once
#include "win32-window-class.h"
#include <memory>

class Win32Window final
{
public:
	static void PollMessages();
	static bool IsAnyWindowRunning();

	void Destroy();
	const void*& GetHandle() const;
	bool IsRunning() const;
	bool TryCreate();
private:
	friend LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static Win32WindowClass s_windowClass;
	static unsigned int s_registeredWindowsCount;
	static unsigned int s_runningWindowsCount;
	static void Unregister();
	static bool TryRegister();

	HWND m_hwnd = nullptr;
	bool m_isRunning = false;
	void SetRunning(bool value);
	bool TryCreateAndShow();
};
