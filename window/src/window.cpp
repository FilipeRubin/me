#include "window.h"
#include "win32/win32-window.h"
#include <Windows.h>

std::vector<Window*> Window::s_windows = std::vector<Window*>();

bool Window::IsAnyWindowRunning()
{
	return Win32Window::IsAnyWindowRunning();
}

void Window::PollMessages()
{
	Win32Window::PollMessages();
}

Window::Window() :
	m_win32window(new Win32Window())
{
	s_windows.emplace_back(this);
}

Window::~Window()
{
	s_windows.erase(std::remove(s_windows.begin(), s_windows.end(), this), s_windows.end());
	delete m_win32window;
}

void Window::Destroy()
{
	m_win32window->Destroy();
}

const void*& Window::GetHandle() const
{
	return m_win32window->GetHandle();
}

bool Window::IsRunning() const
{
	return m_win32window->IsRunning();
}

bool Window::TryCreate()
{
	if (not m_win32window->TryCreate())
	{
		return false;
	}
	return true;
}
