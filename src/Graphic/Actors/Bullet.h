#pragma once
#include "Actor.h"

class Bullet : public Actor
{
public:
	//Inherit Constructor
	//using Actor::Actor;
	Bullet(	glm::vec3 Location,	glm::vec3 Rotation,	glm::vec3 Scale );

	void SetVelocity(float newVelocity);
	void SetLifeTime(float newLifeTime);
	void SetTimeToSpawn(float newTimeToSpawn);

	void Tick(float DeltaTime) override;
	void Draw() override;
	void FaceDirection();
private:
	bool bIsReadyToDraw = false;
	float TimeToSpawn = 2.0f;
	float LifeTime = 10.0f;

};
