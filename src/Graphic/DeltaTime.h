#pragma once
#include "ErrorManager.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class DeltaTime
{
public:
	//Should be called only ONCE per frame!
	static void Update();

	inline static float Get() { return deltatime; };
private:
	static float deltatime;

	static float CurrentTime;
	static float OldTime;

	// Disallow to create an instance of this class
	DeltaTime() {};
};