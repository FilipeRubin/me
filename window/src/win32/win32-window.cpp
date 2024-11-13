#include "win32-window.h"
#include <Windows.h>
#include <iostream>

Win32WindowClass Win32Window::s_windowClass = Win32WindowClass();
unsigned int Win32Window::s_runningWindowsCount = 0U;

void Win32Window::PollGeneralMessages()
{
	MSG msg = {};
	while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

unsigned int Win32Window::GetRunningWindowsCount()
{
	return s_runningWindowsCount;
}

void Win32Window::Destroy()
{
	DestroyWindow((HWND)m_hwnd);
	m_hwnd = NULL;
	DecreaseCounter();
}

const std::string& Win32Window::GetLastErrorInformation() const
{
	return m_lastErrorInfo;
}

bool Win32Window::IsRunning() const
{
	return m_hwnd != NULL;
}

void Win32Window::PollWindowMessages()
{
	MSG msg = {};
	while (PeekMessage(&msg, (HWND)m_hwnd, 0U, 0U, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

bool Win32Window::TryCreate()
{
	if (not TryIncreaseCounter())
	{
		m_lastErrorInfo = "Could not register window class.";
		return false;
	}

	if (not TryCreateAndShow())
	{
		DecreaseCounter();
		return false;
	}

	return true;
}

void Win32Window::DecreaseCounter()
{
	s_runningWindowsCount--;
	if (s_runningWindowsCount == 0U)
	{
		s_windowClass.Unregister();
	}
}

bool Win32Window::TryIncreaseCounter()
{
	if (s_runningWindowsCount == 0U)
	{
		if (not s_windowClass.TryRegister(L"GameWindow"))
		{
			return false;
		}
	}
	s_runningWindowsCount++;
	return true;
}

bool Win32Window::TryCreateAndShow()
{
	m_hwnd = CreateWindow(
		MAKEINTATOM(s_windowClass.GetAtom()),
		L"Game window",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		640, 480,
		NULL,
		NULL,
		(HINSTANCE)s_windowClass.GetHandle(),
		NULL
	);

	if (m_hwnd != NULL)
	{
		SetWindowLongPtr((HWND)m_hwnd, GWLP_USERDATA, (LONG_PTR)this);
		ShowWindow((HWND)m_hwnd, SW_SHOW);
		return true;
	}

	m_lastErrorInfo = "Could not create window.";
	return false;
}
