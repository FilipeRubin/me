#include "window.h"
#include "win32/win32-window.h"

std::vector<class Win32Window*> Window::s_windows = std::vector<class Win32Window*>();

void Window::Process()
{
	while (Win32Window::GetRunningWindowsCount() > 0U)
	{
		Win32Window::PollGeneralMessages();
		for (Win32Window* window : s_windows)
		{
			if (window->IsRunning())
				window->PollWindowMessages();
		}
	}
}

Window::Window(std::unique_ptr<ILogger>&& logger) :
	m_window(new Win32Window()),
	m_logger(std::move(logger))
{
	s_windows.emplace_back(m_window);
}

Window::~Window()
{
	s_windows.erase(std::remove(s_windows.begin(), s_windows.end(), m_window), s_windows.end());
	delete m_window;
}

bool Window::TryInitialize()
{
	if (not m_window->TryCreate(parameters))
	{
		m_logger->LogError(m_window->GetLastErrorInformation());
		return false;
	}
	m_logger->LogInfo("Window created successfully.");
	return true;
}
