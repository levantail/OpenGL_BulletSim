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

	static void GLAPIENTRY
		MessageCallback(GLenum source,
			GLenum type,
			GLuint id,
			GLenum severity,
			GLsizei length,
			const GLchar* message,
			const void* userParam)
	{
		if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
			return;

		fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
			(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
			type, severity, message);
	};
};
