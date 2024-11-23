# Minimal and Efficient
This is a project with the goal of making a 3D renderer without using any premade libraries, relying only on the Windows API. It should support OpenGL for rendering and be able to be used for making games (long-term goal).

The solution is split-up into projects that handle specific tasks. A project may not depend on another, but pieces of one can be "glued" onto another using abstractions or primitive types (such as pointers).

The API is made in a way so that anyone with a basic knowledge of C++ can understand what it does (not necessarily _how_ it does) and create applications with it. For instance, this is how a simple window can be created:
```c++
int main()
{
	// Object that encapsulates windowing functionality
	Window window;
	
	// Try to initialize the window
	if (not window.TryCreate())
		return -1;
	
	// Run loop while there's at least one window running
	while (Window::IsAnyWindowRunning())
	{
		// Process messages for all running windows (only one in this case)
		Window::PollMessages();
	}
	
	// Destroy the window after the application is closed
	window.Destroy();
}
```

## Technical targets
* C++
* Visual Studio (2022)
* Windows
* OpenGL
* 64-bits

## How to compile
If you're in Visual Studio 2022, it should be pretty straightforward as you can execute the solution file and compile everything from there.

If you're using another compiler or IDE, you need to compile every file contained within each project's `src` folder as a static (or dynamic) library. The `game` project is the only one that should be compiled as an executable. You can also create a new project from scratch instead of using `game` if you want to make your own use of it.
