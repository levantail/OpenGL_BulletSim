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

#include "ThreadManager.h"

//#include <random>


class BulletManager
{
public:
	BulletManager() {

		threadsPool = std::make_unique<ThreadManager>();
	};

	void Update(float time);

	//Creates an bullet with specified parameters
	void Fire(glm::vec2 pos, glm::vec2 dir, float speed, float time, float life_time);

	void SetWindowSize(int Width, int Height) { W_WIDTH = (float)Width; W_HEIGHT = (float)Height; };
private:
	void DrawObjects();

	void TickWallMT(std::shared_ptr<Wall>& singleWall, float time);
	void TickBulletMT(std::shared_ptr<Bullet>& singleBullet, float time);

	float W_WIDTH = 0;
	float W_HEIGHT = 0 ;

	//void simplejob();
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
	std::vector<std::shared_ptr<Wall>> BufferWalls = {};
	std::vector<std::shared_ptr<Bullet>> BufferBullets = {};

	//Thread Pool
	std::unique_ptr<ThreadManager> threadsPool;

	//Checks every bullet for collisions with any wall or window edge
	//If it true - reflect according to situation:
	// case window: inverse x or y direction depending on where obj cross a border
	// case wall: reflect bullet depending on his direction vector and wall rotation
	void CalculateCollisions(std::shared_ptr<Bullet>& singleBullet);

	//Simply generate some amount of objects every frame.
	//Used imgui interface to adjust parameters
	void GenerateWalls(float DeltaTime);
	void GenerateBullets(float DeltaTime);

	void DrawUI();

	std::mutex mu1;
	std::mutex mu2;
	std::mutex mu3;
};