#pragma once
#include <vector>

class Window
{
public:
	static bool IsAnyWindowRunning();
	static void PollMessages();

	Window();
	~Window();
	void Destroy();
	const void*& GetHandle() const;
	bool IsRunning() const;
	bool TryCreate();
private:
	static std::vector<Window*> s_windows;

	class Win32Window* m_win32window;
};
