#include <console-logger.h>
#include <opengl-graphics-context.h>
#include <window.h>

int main()
{
	Window window(std::make_unique<ConsoleLogger>());

	if (not window.TryInitialize(std::make_unique<OpenGLGraphicsContext>()))
		return -1;

	Window::Process();

	return 0;
}
