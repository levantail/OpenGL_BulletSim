#include "Bullet.h"

Bullet::Bullet(glm::vec3 Location, glm::vec3 Rotation, glm::vec3 Scale) : Actor(Location, Rotation, Scale)
{
	Vertices =
	{
		//	X		Y	  S		T
			-1.0f, -1.0f, 0.0f, 0.0f,
			-1.0f,  2.0f, 0.0f, 0.7f,
			 0.0f,  3.0f, 0.5f, 1.0f,
			 1.0f,  2.0f, 1.0f, 0.7f,
			 1.0f, -1.0f, 1.0f, 0.0f
	};

	Indices =
	{
		0, 1, 3,
		0, 4, 3,
		1, 2, 3
	};
}

//Bullet::Actor()
//{
//
//}

void Bullet::SetVelocity(float newVelocity)
{
	this->Velocity = newVelocity;
}


void Bullet::SetLifeTime(float newLifeTime)
{
	this->LifeTime = newLifeTime;
}

void Bullet::SetTimeToSpawn(float newTimeToSpawn)
{
	this->TimeToSpawn = newTimeToSpawn;
}

void Bullet::Tick(float DeltaTime)
{
	//Check is time to 'spawn'.
	if (TimeToSpawn > 0)
	{
		TimeToSpawn -= DeltaTime::Get();
		return;
	}

	if (LifeTime > 0)
	{
		LifeTime -= DeltaTime::Get();
	}
	else
	{
		Destroy();
		return;
	}
	__super::Tick(DeltaTime);

	//Movement

	Transform.Location +=  Direction * (Velocity * DeltaTime::Get());
}

void Bullet::FaceDirection()
{
	float rot = glm::atan(Direction.y, Direction.x);
	rot = (rot * 180) / glm::pi<float>();
	Transform.Rotation = glm::vec3(0.0, 0.0, rot - 90);
}
