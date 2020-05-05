#include "RenderWindow.h"

bool RenderWindow::Initialize(std::string WindowTitle, int WindowWidth, int Window_Height)
{
	/* Initialize the library */
	if (!glfwInit())
		return false;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//In case of MAC X
#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	/* Create a windowed mode window and its OpenGL context */
	AppWindow = glfwCreateWindow(WindowWidth, Window_Height, WindowTitle.c_str(), NULL, NULL);
	if (AppWindow == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		return false;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(AppWindow);

	glfwSwapInterval(1); // for v-sync

	if (glewInit() != GLEW_OK) {
		std::cout << "Error!" << std::endl;
		return false;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;

	//Blending function for alpha channel.
	//GLCall(glEnable(GL_DEPTH_TEST));

	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	return true;
}

GLFWwindow* RenderWindow::GetGLFWwindow()
{
	return AppWindow;
}

void RenderWindow::callBack_FrameBufferSize(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
