#pragma once
#include <Windows.h>

namespace OpenGLHelpers
{
	HGLRC CreateContext(HDC& hdc);
	bool TryInitialize(const HINSTANCE& hInstance);
}
