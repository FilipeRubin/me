#include <console-logger.h>
#include <opengl-graphics-context.h>
#include <window.h>

int main()
{
	Window windows[10]
	{ 
		{std::make_unique<ConsoleLogger>()},
		{std::make_unique<ConsoleLogger>()},
		{std::make_unique<ConsoleLogger>()},
		{std::make_unique<ConsoleLogger>()},
		{std::make_unique<ConsoleLogger>()},
		{std::make_unique<ConsoleLogger>()},
		{std::make_unique<ConsoleLogger>()},
		{std::make_unique<ConsoleLogger>()},
		{std::make_unique<ConsoleLogger>()},
		{std::make_unique<ConsoleLogger>()}
	};

	for (Window& window : windows)
		if (not window.TryInitialize(std::make_unique<OpenGLGraphicsContext>()))
			return -1;

	Window::Process();

	return 0;
}
