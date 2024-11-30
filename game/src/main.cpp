#include <window/window.h>
#include <graphics/opengl-graphics-context.h>

int main()
{
	Window window;
	OpenGLGraphicsContext graphicsContext = OpenGLGraphicsContext( window.GetHandle() );

	if (not window.TryCreate())
		return -1;

	if (not graphicsContext.TryCreate())
	{
		window.Destroy();
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

		Window::PollMessages();
	}

	graphicsContext.Destroy();
	window.Destroy();

	return 0;
}
