#pragma once
#include <i-logger.h>
#include <memory>
#include <vector>

class Window
{
public:
	static void Process();
	Window(std::unique_ptr<ILogger>&& logger);
	~Window();
	bool IsRunning() const;
	bool TryInitialize();
private:
	static std::vector<class Win32Window*> s_windows;
	class Win32Window* m_window;
	std::unique_ptr<ILogger> m_logger;
};
