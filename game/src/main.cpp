#include <window.h>
#include <opengl-graphics-context.h>

int main()
{
	Window window;
	Window window2;
	OpenGLGraphicsContext graphicsContext = OpenGLGraphicsContext( window.GetHandle() );
	OpenGLGraphicsContext graphicsContext2 = OpenGLGraphicsContext( window2.GetHandle() );

	if (not window.TryCreate() or not window2.TryCreate())
		return -1;

	if (not graphicsContext.TryInitialize() or not graphicsContext2.TryInitialize())
	{
		window.Destroy();
		window2.Destroy();
		return -2;
	}

	while (Window::IsAnyWindowRunning())
	{
		if (window.IsRunning())
		{
			graphicsContext.MakeCurrent();
			graphicsContext.DrawTriangle(1.0, 1.0f, 0.0f);
			graphicsContext.Present();
		}
		if (window2.IsRunning())
		{
			graphicsContext2.MakeCurrent();
			graphicsContext2.DrawTriangle(0.0, 1.0f, 1.0f);
			graphicsContext2.Present();
		}

		Window::PollMessages();
	}

	graphicsContext.Terminate();
	graphicsContext2.Terminate();
	window.Destroy();
	window2.Destroy();

	return 0;
}
