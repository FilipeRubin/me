#pragma once

class GraphicsContext final
{
public:
	bool TryInitialize(void* windowHandle);
	void Terminate();
};
