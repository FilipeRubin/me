# Minimal and Efficient
This is a project with the goal of making a 3D renderer without using any premade libraries, relying only on the Windows API. It should support OpenGL for rendering and be able to be used for making games (long-term goal).
The solution is split-up into projects that handle specific tasks. A project may depend on another, but it should do it using abstractions (except when it means extra unnecessary performance overhead).
The API is made in a way that anyone with a basic knowledge of C++ can understand what it does (not necessarily _how_ it does) and create applications with it. For instance, this is how a simple window can be created:
```c++
int main()
{
	unique_ptr<ILogger> logger = make_unique<ConsoleLogger>(); // Object that implements logging functionality
	Window window(move(logger)); // Object that encapsulates windowing functionality (and uses the logger)

	if (not window.TryInitialize()) // Try to initialize the window
		return -1;

	Window::Process(); // Run every active window loop (support multiple windows)

	return 0; // Returns when every window is closed
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
