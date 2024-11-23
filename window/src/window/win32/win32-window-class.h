#pragma once
#include <Windows.h>

class Win32WindowClass final
{
public:
	unsigned short GetId() const;
	HINSTANCE GetInstanceHandle() const;
	bool IsActive() const;
	bool TryRegister(const wchar_t* className);
	void Unregister();
private:
	HINSTANCE m_hInstance;
	unsigned short m_classId;
};
