#pragma once
#include "win32-window-class.h"
#include "window-parameters.h"
#include <i-graphics-context.h>
#include <memory>

class Win32Window
{
public:
	static void PollGeneralMessages();
	static unsigned int GetRunningWindowsCount();
	Win32Window();
	void Destroy();
	const std::string& GetLastErrorInformation() const;
	const IGraphicsContext& GetGraphicsContext() const;
	bool IsRunning() const;
	void PollWindowMessages();
	bool TryCreate(const WindowParameters& parameters, std::unique_ptr<IGraphicsContext>&& graphicsContext);
private:
	static Win32WindowClass s_windowClass;
	static unsigned int s_runningWindowsCount;
	static void DecreaseCounter();
	static bool TryIncreaseCounter();
	void* m_hwnd;
	std::unique_ptr<IGraphicsContext> m_graphicsContext;
	std::string m_lastErrorInfo;
	bool TryCreateAndShow(const WindowParameters& parameters);
};
