//OpenGL GLEW
#include "Engine.h"


const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 820;

//"Bullet Manager Task"
int main(void)
{
	//TODO:
	// + Cache function bind calls to opengl API
	// + multi-threading at creating new objects and at collision calculation
	// + Auto-mode to fit desired performance

	Engine engine;

	//Engine Start
	if (engine.Initialize("Bullet Manager Task", SCR_WIDTH, SCR_HEIGHT)) {
		while (!engine.ProceedMessage())
		{
			engine.RenderFrame();
		}
	}

	engine.TerminateAndClose();
	return 0;
}