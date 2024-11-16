#pragma once
#include "window-parameters.h"
#include <i-graphics-context.h>
#include <i-logger.h>
#include <memory>
#include <vector>

class Window
{
public:
	static void Process();
	WindowParameters parameters;
	Window(std::unique_ptr<ILogger>&& logger);
	~Window();
	bool TryInitialize(std::unique_ptr<IGraphicsContext>&& graphicsContext);
private:
	static std::vector<class Win32Window*> s_windows;
	class Win32Window* m_window;
	std::unique_ptr<ILogger> m_logger;
};
