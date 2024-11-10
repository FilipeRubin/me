#pragma once
#include <vector>

class Window
{
public:
	static void Process();
	Window();
	~Window();
	bool IsRunning() const;
	bool TryInitialize();
private:
	static std::vector<class Win32Window*> s_windows;
	class Win32Window* m_window;
};
