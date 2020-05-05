#pragma once
#include "RenderWindow.h"
#include "Graphic/Graphic.h"

class Engine {
protected:

	GLFWwindow* AppWindow;
	RenderWindow GLRenderWindow;
	Graphics Graphic;

public:
	bool Initialize(std::string WindowTitle, int WindowWidth, int Window_Height);

	//Loop function that constantly checks glfw windows for valid status
	bool ProceedMessage();
	void RenderFrame();
	void TerminateAndClose();

private:
	std::string WindowTitle = "";

	// Keyboard or mouse input processing
	void ProcessInput();
};
