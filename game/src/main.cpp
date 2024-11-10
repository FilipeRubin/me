#include <window.h>
#include <console-logger.h>

int main()
{
	Window window(std::make_unique<ConsoleLogger>());

	if (not window.TryInitialize())
		return -1;

	Window::Process();

	return 0;
}
