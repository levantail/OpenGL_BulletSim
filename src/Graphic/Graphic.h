#pragma once
#include "../BulletManager.h"
#include "../RenderWindow.h"
#include "DeltaTime.h"

#include <vector>

class Graphics
{
public:

	bool Initialize(GLFWwindow* newAppWindow);

	void RenderFrame();

	void Terminate();

	void SetAppWindow(GLFWwindow* newAppWindow);


protected:
	GLFWwindow* AppWindow;

	int W_WIDTH = 0;
	int W_HEIGHT = 0;

	//Perform frame clean and prepare for new frame calculation
	void PrepareFrame();

	//All calculations is done, swap buffers and end frame
	void EndFrame();

	void DrawUI();

	std::unique_ptr<BulletManager> bulletManager;

};
