#include "win32-window.h"
#include <Windows.h>

Win32WindowClass Win32Window::s_windowClass = Win32WindowClass();
unsigned int Win32Window::s_runningWindowsCount = 0U;

static std::wstring StringToWideString(const std::string& source)
{
	if (source.empty())
	{
		return std::wstring();
	}

	size_t length = MultiByteToWideChar(CP_UTF8, 0, source.c_str(), (int)source.length(), 0, 0);
	std::wstring result(length, L'\0');
	MultiByteToWideChar(CP_UTF8, 0, source.c_str(), (int)source.length(), &result[0], (int)result.length());
	return result;
}

void Win32Window::PollGeneralMessages()
{
	MSG msg = {};
	while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

unsigned int Win32Window::GetRunningWindowsCount()
{
	return s_runningWindowsCount;
}

Win32Window::Win32Window() :
	m_hwnd(nullptr)
{
}

void Win32Window::Destroy()
{
	m_graphicsContext->Terminate();
	DestroyWindow((HWND)m_hwnd);
	m_hwnd = NULL;
	DecreaseCounter();
}

const std::string& Win32Window::GetLastErrorInformation() const
{
	return m_lastErrorInfo;
}

const IGraphicsContext& Win32Window::GetGraphicsContext() const
{
	return *m_graphicsContext.get();
}

bool Win32Window::IsRunning() const
{
	return m_hwnd != NULL;
}

void Win32Window::PollWindowMessages()
{
	MSG msg = {};
	while (PeekMessage(&msg, (HWND)m_hwnd, 0U, 0U, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

bool Win32Window::TryCreate(const WindowParameters& parameters, std::unique_ptr<IGraphicsContext>&& graphicsContext)
{
	if (not TryIncreaseCounter())
	{
		m_lastErrorInfo = "Could not register window class";
		return false;
	}

	if (not TryCreateAndShow(parameters))
	{
		DecreaseCounter();
		return false;
	}

	if (not graphicsContext->TryInitialize(m_hwnd))
	{
		m_lastErrorInfo = "Could not initialize graphics context";
		Destroy();
		return false;
	}

	m_graphicsContext = std::move(graphicsContext);

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

bool Win32Window::TryCreateAndShow(const WindowParameters& parameters)
{
	DWORD windowStyle = parameters.isResizable ? WS_OVERLAPPEDWINDOW : WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX);

	RECT windowRect = { 0, 0, parameters.width, parameters.height };
	if (AdjustWindowRect(&windowRect, windowStyle, FALSE) == FALSE)
	{
		m_lastErrorInfo = "Could not size window correctly.";
		return false;
	}

	m_hwnd = CreateWindow(
		MAKEINTATOM(s_windowClass.GetAtom()),
		StringToWideString(parameters.name).c_str(),
		windowStyle,
		CW_USEDEFAULT, CW_USEDEFAULT,
		windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
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
