#include "DeltaTime.h"

float DeltaTime::deltatime = 0.0f;

float DeltaTime::CurrentTime = 0.0f;

float DeltaTime::OldTime = 0.0f;

void DeltaTime::Update()
{
	CurrentTime = (float)glfwGetTime();
	deltatime = CurrentTime - OldTime;
	OldTime = CurrentTime;
}


