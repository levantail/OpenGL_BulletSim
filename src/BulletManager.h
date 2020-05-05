#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

#include "Graphic/ErrorManager.h"

#include "Graphic/Actors/Actor.h"
#include "Graphic/Actors/Bullet.h"
#include "Graphic/Actors/Wall.h"
#include "Graphic/Actors/HelperManager.h"

class BulletManager
{
public:

	void Update(float time);

	//Creates an bullet with specified parameters
	void Fire(glm::vec2 pos, glm::vec2 dir, float speed, float time, float life_time);

	void SetWindowSize(int Width, int Height) { W_WIDTH = (float)Width; W_HEIGHT = (float)Height; };
private:

	float W_WIDTH = 0;
	float W_HEIGHT = 0 ;


	//Wall object generation properties
	int WallMaxAmount = 100;
	float WallSpawnRate = 1.0f;
	int WallSpawnAmount = 10;
	//Cache used for next frame object generation
	//In case if all values will be pretty small 
	//and framerate big
	// this helps to handle 1-2 obj. generation per second.
	float WallSpawnCache = .0f;

	//Bullet object generation properties
	int BulletMaxAmount = 10;
	float BulletSpawnRate = 1.0f;
	int BulletSpawnAmount = 3;
	float BulletSpawnCache = .0f;


	//Buffers for walls and bullets objects
	//Both could be stored as Actor and then downcast to desired obj.
	std::vector<std::unique_ptr<Wall>> BufferWalls = {};
	std::vector<std::unique_ptr<Bullet>> BufferBullets = {};

	//Checks every bullet for collisions with any wall or window edge
	//If it true - reflect according to situation:
	// case window: inverse x or y direction depending on where obj cross a border
	// case wall: reflect bullet depending on his direction vector and wall rotation
	void CalculateCollisions();

	//Simply generate some amount of objects every frame.
	//Used imgui interface to adjust parameters
	void GenerateWalls(float DeltaTime);
	void GenerateBullets(float DeltaTime);

	void DrawUI();

};