#pragma once
#include "i-graphics-context.h"

class OpenGLGraphicsContext final : public IGraphicsContext
{
public:
	OpenGLGraphicsContext(const void*& windowHandle);
	void MakeCurrent() const override;
	void Present() const override;
	bool TryInitialize() override;
	void Terminate() override;

	void DrawTriangle(float r, float g, float b); // TEMPORARY METHOD TO TEST DRAWING ONTO THE SCREEN
private:
	const void*& m_hwnd;
	void* m_hdc = nullptr;
	void* m_hglrc = nullptr;
	bool TryConfigurePixelFormat();
};
