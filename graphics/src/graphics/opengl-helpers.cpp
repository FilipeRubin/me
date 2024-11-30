#include "opengl-helpers.h"

#define GL_TRUE 1

#define WGL_CONTEXT_MAJOR_VERSION_ARB     0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB     0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB      0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB  0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002

#define WGL_DRAW_TO_WINDOW_ARB            0x2001
#define WGL_SUPPORT_OPENGL_ARB            0x2010
#define WGL_DOUBLE_BUFFER_ARB             0x2011
#define WGL_PIXEL_TYPE_ARB                0x2013
#define WGL_COLOR_BITS_ARB                0x2014
#define WGL_ALPHA_BITS_ARB                0x201B
#define WGL_DEPTH_BITS_ARB                0x2022
#define WGL_STENCIL_BITS_ARB              0x2023
#define WGL_TYPE_RGBA_ARB                 0x202B

typedef BOOL(WINAPI* PFNWGLCHOOSEPIXELFORMATARBPROC) (HDC hdc, const int* piAttribIList, const FLOAT* pfAttribFList, UINT nMaxFormats, int* piFormats, UINT* nNumFormats);
typedef HGLRC(WINAPI* PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC hDC, HGLRC hShareContext, const int* attribList);
static PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = nullptr;
static PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;

static bool s_glInitialized = false;

HGLRC OpenGLHelpers::CreateContext(HDC& hdc)
{
	const int pixelFormatAttribs[]
	{
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		WGL_COLOR_BITS_ARB, 24,
		WGL_ALPHA_BITS_ARB, 8,
		WGL_DEPTH_BITS_ARB, 24,
		WGL_STENCIL_BITS_ARB, 8,
		0
	};

	int pixelFormat = 0;
	UINT numPixelFormats;

	wglChoosePixelFormatARB(hdc, pixelFormatAttribs, NULL, 1U, &pixelFormat, &numPixelFormats);

	if (pixelFormat == 0 or numPixelFormats == 0U)
	{
		return NULL;
	}

	PIXELFORMATDESCRIPTOR pfd = { 0 };

	if (SetPixelFormat(hdc, pixelFormat, &pfd) == FALSE)
	{
		return NULL;
	}

	const int contextAttribs[]
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 6,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
	};

	return wglCreateContextAttribsARB(hdc, NULL, contextAttribs);
}

bool OpenGLHelpers::TryInitialize(const HINSTANCE& hInstance)
{
	if (s_glInitialized)
	{
		return true;
	}

	WNDCLASS wc = {};
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = DefWindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = NULL;
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"FalseOpenGLContext";

	ATOM classId = RegisterClass(&wc);
	if (classId == NULL)
	{
		return false;
	}

	HWND hwnd = CreateWindow(
		MAKEINTATOM(classId),
		L"Dummy window",
		WS_DISABLED,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL
	);

	if (hwnd == NULL)
	{
		UnregisterClass(MAKEINTATOM(classId), hInstance);
		return false;
	}

	HDC hdc = GetDC(hwnd);

	PIXELFORMATDESCRIPTOR pfd = { 0 };
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cAlphaBits = 8;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int pixelFormat = ChoosePixelFormat(hdc, &pfd);

	if (pixelFormat == NULL)
	{
		ReleaseDC(hwnd, hdc);
		DestroyWindow(hwnd);
		UnregisterClass(MAKEINTATOM(classId), hInstance);
		return false;
	}

	if (SetPixelFormat(hdc, pixelFormat, &pfd) == FALSE)
	{
		ReleaseDC(hwnd, hdc);
		DestroyWindow(hwnd);
		UnregisterClass(MAKEINTATOM(classId), hInstance);
		return false;
	}

	HGLRC hglrc = wglCreateContext(hdc);

	if (hglrc == NULL)
	{
		ReleaseDC(hwnd, hdc);
		DestroyWindow(hwnd);
		UnregisterClass(MAKEINTATOM(classId), hInstance);
		return false;
	}

	wglMakeCurrent(hdc, hglrc);
	
	wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
	wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hglrc);
	ReleaseDC(hwnd, hdc);
	DestroyWindow(hwnd);
	UnregisterClass(MAKEINTATOM(classId), hInstance);

	if (wglChoosePixelFormatARB == nullptr or wglCreateContextAttribsARB == nullptr)
	{
		return false;
	}

	s_glInitialized = true;
	return true;
}
