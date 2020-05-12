#include "Graphic.h"

bool Graphics::Initialize(GLFWwindow* newAppWindow)
{
	this->AppWindow = newAppWindow;

	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(AppWindow, true);
	ImGui::StyleColorsDark();

	glfwGetWindowSize(AppWindow, &W_WIDTH, &W_HEIGHT);

	bulletManager = std::make_unique<BulletManager>();
	bulletManager->SetWindowSize(W_WIDTH, W_HEIGHT);

	float x, y;
	float angle;
	
	for (size_t i = 0; i < 10; ++i)
	{
		x = HelpManager::randomf(10.0f, (float)W_WIDTH - 10.0f);
		y = HelpManager::randomf(10.0f, (float)W_HEIGHT - 10.0f);

		bulletManager->Fire(
			glm::vec2(x, y),
			glm::vec2(HelpManager::randomf(-1.0f, 1.0f), HelpManager::randomf(-1.0f, 1.0f)),
			HelpManager::randomf(200.0f, 300.0f),
			HelpManager::randomf(1.0f, 10.0f),
			HelpManager::randomf(3.0f, 10.0f)
		);
	}

	return true;
}

void Graphics::RenderFrame()
{
	PrepareFrame();

	DrawUI();

	bulletManager->Update(DeltaTime::Get());

	EndFrame();
}

void Graphics::Terminate()
{
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	bulletManager.reset();
}

void Graphics::SetAppWindow(GLFWwindow* newAppWindow)
{
	this->AppWindow = newAppWindow;
}

void Graphics::PrepareFrame()
{
	Renderer::Clear();
	ImGui_ImplGlfwGL3_NewFrame();

	DeltaTime::Update();
}

void Graphics::EndFrame()
{
	ImGui::Render();
	ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

	/* Swap front and back buffers */
	GLCall(glfwSwapBuffers(AppWindow));

	/* Poll for and process events */
	GLCall(glfwPollEvents());
}

void Graphics::DrawUI()
{
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}
