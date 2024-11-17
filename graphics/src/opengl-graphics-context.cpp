#include "opengl-graphics-context.h"
#include <Windows.h>

OpenGLGraphicsContext* OpenGLGraphicsContext::s_currentContext = nullptr;

OpenGLGraphicsContext::OpenGLGraphicsContext() :
	m_hwnd(nullptr),
	m_hdc(nullptr),
	m_hglrc(nullptr)
{
}

void OpenGLGraphicsContext::Present() const
{
	SwapBuffers((HDC)m_hdc);
}

void OpenGLGraphicsContext::SetContext() const
{
	if (s_currentContext != this)
	{
		wglMakeCurrent((HDC)m_hdc, (HGLRC)m_hglrc);
		s_currentContext = const_cast<OpenGLGraphicsContext*>(this);
	}
}

bool OpenGLGraphicsContext::TryInitialize(void* windowHandle)
{
	m_hwnd = windowHandle;
	m_hdc = GetDC((HWND)m_hwnd);

	if (m_hdc == NULL)
	{
		return false;
	}

	if (not TryConfigurePixelFormat())
	{
		return false;
	}

	m_hglrc = wglCreateContext((HDC)m_hdc);

	if (m_hglrc == NULL)
	{
		ReleaseDC((HWND)m_hwnd, (HDC)m_hdc);
		return false;
	}

	return true;
}

void OpenGLGraphicsContext::Terminate()
{
	if (s_currentContext == this)
	{
		wglMakeCurrent(NULL, NULL);
	}
	wglDeleteContext((HGLRC)m_hglrc);
	ReleaseDC((HWND)m_hwnd, (HDC)m_hdc);
}

bool OpenGLGraphicsContext::TryConfigurePixelFormat()
{
	PIXELFORMATDESCRIPTOR pfd = { 0 };
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int pixelFormat = ChoosePixelFormat((HDC)m_hdc, &pfd);

	if (pixelFormat == NULL)
	{
		ReleaseDC((HWND)m_hwnd, (HDC)m_hdc);
		return false;
	}

	if (SetPixelFormat((HDC)m_hdc, pixelFormat, &pfd) == FALSE)
	{
		ReleaseDC((HWND)m_hwnd, (HDC)m_hdc);
		return false;
	}

	return true;
}