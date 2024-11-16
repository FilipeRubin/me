#pragma once

class IGraphicsContext
{
public:
	virtual ~IGraphicsContext() = default;
	virtual void Present() const = 0;
	virtual void SetContext() const = 0;
	virtual bool TryInitialize(void* windowHandle) = 0;
	virtual void Terminate() = 0;
};
