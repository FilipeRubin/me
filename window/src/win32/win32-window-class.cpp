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
		window->Destroy();
		return 0;
	}
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

unsigned short Win32WindowClass::GetAtom() const
{
	return m_registeredAtom;
}

void* Win32WindowClass::GetHandle() const
{
	return m_hInstance;
}

bool Win32WindowClass::IsActive() const
{
	return m_registeredAtom != NULL;
}

bool Win32WindowClass::TryRegister(const wchar_t* className)
{
	m_hInstance = GetModuleHandle(NULL);
	if (m_hInstance == NULL)
		return false;

	WNDCLASS wc = {};
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = (HINSTANCE)m_hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = className;

	m_registeredAtom = RegisterClass(&wc);
	if (m_registeredAtom == NULL)
		return false;

	return true;
}

void Win32WindowClass::Unregister()
{
	BOOL result = UnregisterClass(MAKEINTATOM(m_registeredAtom), (HINSTANCE)m_hInstance);
	assert(result);
}
