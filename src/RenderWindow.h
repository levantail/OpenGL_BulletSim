#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>



#include "Graphic/ErrorManager.h"
#include "RenderWindow.h"
#include <string>
#include <iostream>

class RenderWindow
{
protected:
	GLFWwindow* AppWindow;

public:

	bool Initialize(std::string WindowTitle, int WindowWidth, int Window_Height);

	GLFWwindow* GetGLFWwindow();

private:
	static void callBack_FrameBufferSize(GLFWwindow* window, int width, int height);

};
