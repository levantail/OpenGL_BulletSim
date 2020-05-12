#include "Engine.h"

bool Engine::Initialize(std::string WindowTitle, int WindowWidth, int Window_Height)
{
	// During init, enable debug output


	if (!GLRenderWindow.Initialize(WindowTitle, WindowWidth, Window_Height)) 
	{
		std::cout << "Failed to initialize render window" << std::endl;
		return false;
	}

	this->WindowTitle = WindowTitle;
	this->AppWindow = GLRenderWindow.GetGLFWwindow();

	if (!Graphic.Initialize(AppWindow)) {
		std::cout << "Failed to initialize graphic" << std::endl;
		return false;
	}

	//glEnable(GL_DEBUG_OUTPUT);
	//glDebugMessageCallback(MessageCallback, 0);

	return true;
}

bool Engine::ProceedMessage()
{
	ProcessInput();
	return glfwWindowShouldClose(GLRenderWindow.GetGLFWwindow());
}

void Engine::RenderFrame()
{
	Graphic.RenderFrame();
}

void Engine::TerminateAndClose()
{
	Graphic.Terminate();
	glfwTerminate();

}

void Engine::ProcessInput()
{
	//Press ESC to close Application
	if (glfwGetKey(AppWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(AppWindow, true);
	}
}
