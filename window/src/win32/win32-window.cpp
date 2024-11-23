#include "win32-window.h"

Win32WindowClass Win32Window::s_windowClass = Win32WindowClass();
unsigned int Win32Window::s_registeredWindowsCount = 0U;
unsigned int Win32Window::s_runningWindowsCount = 0U;

void Win32Window::PollMessages()
{
	MSG msg = {};
	while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

bool Win32Window::IsAnyWindowRunning()
{
	return s_runningWindowsCount != 0U;
}

void Win32Window::Destroy()
{
	SetRunning(false);
	if (m_hwnd != NULL)
	{
		DestroyWindow(m_hwnd);
		m_hwnd = NULL;
		Unregister();
	}
}

const void*& Win32Window::GetHandle() const
{
	return (const void*&)m_hwnd;
}

bool Win32Window::IsRunning() const
{
	return m_isRunning;
}

bool Win32Window::TryCreate()
{
	if (not TryRegister())
	{
		return false;
	}

	if (not TryCreateAndShow())
	{
		Unregister();
		return false;
	}

	return true;
}

void Win32Window::Unregister()
{
	s_registeredWindowsCount--;
	if (s_registeredWindowsCount == 0U)
	{
		s_windowClass.Unregister();
	}
}

bool Win32Window::TryRegister()
{
	if (s_registeredWindowsCount == 0U)
	{
		if (not s_windowClass.TryRegister(L"GameWindow"))
		{
			return false;
		}
	}
	s_registeredWindowsCount++;
	return true;
}

void Win32Window::SetRunning(bool value)
{
	if (m_isRunning != value)
	{
		m_isRunning = value;

		s_runningWindowsCount += m_isRunning ? 1 : -1;

		ShowWindow(m_hwnd, m_isRunning ? SW_SHOW : SW_HIDE);
	}
}

bool Win32Window::TryCreateAndShow()
{
	const bool isResizable = true;
	const DWORD windowStyle = isResizable ? WS_OVERLAPPEDWINDOW : WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME;
	const int width = 800;
	const int height = 600;

	RECT windowRect = { 0, 0, width, height };
	if (AdjustWindowRect(&windowRect, windowStyle, FALSE) == FALSE)
	{
		return false;
	}

	m_hwnd = CreateWindowEx(
		NULL,
		MAKEINTATOM(s_windowClass.GetId()),
		L"ME Engine",
		windowStyle,
		CW_USEDEFAULT, CW_USEDEFAULT,
		windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		s_windowClass.GetInstanceHandle(),
		NULL
	);

	if (m_hwnd == NULL)
	{
		return false;
	}

	SetWindowLongPtr(m_hwnd, GWLP_USERDATA, (LONG_PTR)this);
	SetRunning(true);
	return true;
}
