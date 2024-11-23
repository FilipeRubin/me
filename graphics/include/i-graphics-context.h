#pragma once
#include <string>

class IGraphicsContext
{
public:
	virtual ~IGraphicsContext() = default;
	virtual void MakeCurrent() const = 0;
	virtual void Present() const = 0;
	virtual bool TryInitialize() = 0;
	virtual void Terminate() = 0;
};
