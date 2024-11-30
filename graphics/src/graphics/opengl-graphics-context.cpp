#include "graphics/opengl-graphics-context.h"
#include "opengl-helpers.h"
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

bool OpenGLGraphicsContext::TryCreate()
{
	if (not OpenGLHelpers::TryInitialize(GetModuleHandle(NULL)))
	{
		return false;
	}

	m_hdc = GetDC((HWND)m_hwnd);

	if (m_hdc == NULL)
	{
		return false;
	}

	m_hglrc = OpenGLHelpers::CreateContext((HDC&)m_hdc);

	if (m_hglrc == NULL)
	{
		ReleaseDC((HWND)m_hwnd, (HDC)m_hdc);
		return false;
	}

	return true;
}

void OpenGLGraphicsContext::Destroy()
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
