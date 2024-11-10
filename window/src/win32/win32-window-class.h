#pragma once

class Win32WindowClass
{
public:
	unsigned short GetAtom() const;
	void* GetHandle() const;
	bool IsActive() const;
	bool TryRegister(const wchar_t* className);
	void Unregister();
private:
	void* m_hInstance;
	unsigned short m_registeredAtom;
};
