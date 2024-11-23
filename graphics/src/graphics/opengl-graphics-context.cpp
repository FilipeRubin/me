#include "graphics/opengl-graphics-context.h"
#include <Windows.h>
#include <gl/GL.h>

OpenGLGraphicsContext::OpenGLGraphicsContext(const void*& windowHandle) :
	m_hwnd(windowHandle)
{
}

void OpenGLGraphicsContext::MakeCurrent() const
{
	if (wglGetCurrentContext() != m_hglrc)
	{
		wglMakeCurrent((HDC)m_hdc, (HGLRC)m_hglrc);
	}
}

void OpenGLGraphicsContext::Present() const
{
	SwapBuffers((HDC)m_hdc);
}

bool OpenGLGraphicsContext::TryInitialize()
{
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
	if (wglGetCurrentContext() == m_hglrc)
	{
		wglMakeCurrent(NULL, NULL);
	}
	wglDeleteContext((HGLRC)m_hglrc);
	ReleaseDC((HWND)m_hwnd, (HDC)m_hdc);
}

void OpenGLGraphicsContext::DrawTriangle(float r, float g, float b)
{
	glBegin(GL_TRIANGLES);
	glColor3f(r, g, b);
	glVertex2d(0.5f, -0.5f);
	glVertex2d(0.0f, 0.5f);
	glVertex2d(-0.5f, -0.5f);
	glEnd();
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