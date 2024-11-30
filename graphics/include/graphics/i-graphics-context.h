#pragma once
#include <string>

class IGraphicsContext
{
public:
	virtual ~IGraphicsContext() = default;
	virtual void MakeCurrent() const = 0;
	virtual void Present() const = 0;
	virtual bool TryCreate() = 0;
	virtual void Destroy() = 0;
};
