#pragma once

class IGraphicsContext
{
public:
	virtual ~IGraphicsContext() = default;
	virtual bool TryInitialize(void* windowHandle) = 0;
	virtual void Terminate() = 0;
};
