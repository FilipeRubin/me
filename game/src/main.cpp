#include <window.h>

int main()
{
	Window window;

	if (not window.TryInitialize())
		return -1;

	Window::Process();

	return 0;
}
