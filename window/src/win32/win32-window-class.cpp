#include "win32-window-class.h"
#include "win32-window.h"
#include <cassert>
#include <Windows.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
	{
		Win32Window* window = (Win32Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		window->SetRunning(false);
		return 0;
	}
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

unsigned short Win32WindowClass::GetId() const
{
	return m_classId;
}

HINSTANCE Win32WindowClass::GetInstanceHandle() const
{
	return m_hInstance;
}

bool Win32WindowClass::IsActive() const
{
	return m_classId != NULL;
}

bool Win32WindowClass::TryRegister(const wchar_t* className)
{
	m_hInstance = GetModuleHandle(NULL);
	if (m_hInstance == NULL)
		return false;

	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = className;
	wc.hIconSm = NULL;

	m_classId = RegisterClassEx(&wc);
	if (m_classId == NULL)
		return false;

	return true;
}

void Win32WindowClass::Unregister()
{
	UnregisterClass(MAKEINTATOM(m_classId), m_hInstance);
	m_hInstance = nullptr;
	m_classId = NULL;
}
