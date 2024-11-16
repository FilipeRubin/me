#pragma once
#include "i-graphics-context.h"

class OpenGLGraphicsContext final : public IGraphicsContext
{
public:
	OpenGLGraphicsContext();
	void Present() const override;
	void SetContext() const override;
	bool TryInitialize(void* windowHandle) override;
	void Terminate() override;
private:
	static OpenGLGraphicsContext* s_currentContext;
	void* m_hwnd;
	void* m_hdc;
	void* m_hglrc;
};
