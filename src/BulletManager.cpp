#include "BulletManager.h"

void BulletManager::Update(float time)
{
	GenerateWalls(time);
	GenerateBullets(time);

	for (size_t i = 0; i < BufferWalls.size(); ++i)
	{
		threadsPool->push([this, i,time](int)
			{
				this->TickWallMT(std::ref(BufferWalls[i]), time);
			});
	}
	
	for (size_t i = 0; i < BufferBullets.size(); ++i)
	{
		threadsPool->push([this, i, time](int)
			{
				this->TickBulletMT(std::ref(BufferBullets[i]), time);
			});
	}
		
	DrawUI();

	threadsPool->Barrier();

	for (size_t i = 0; i < BufferBullets.size(); ++i)
	{
		threadsPool->push([this, i, time](int)
			{
				this->CalculateCollisions(std::ref(BufferBullets[i]));
			});
	}

	threadsPool->Barrier();

	DrawObjects();
}

void BulletManager::TickWallMT(std::shared_ptr<Wall>& singleWall, float time)
{
	singleWall->Tick(time);
}

void BulletManager::TickBulletMT(std::shared_ptr<Bullet>& singleBullet, float time)
{
	singleBullet->Tick(time);
}

//void BulletManager::simplejob()
//{
//	std::random_device rd;
//	std::mt19937 rng(rd());
//
//	// build a vector of random numbers
//	std::vector<int> data;
//	data.reserve(100000);
//	std::generate_n(std::back_inserter(data), data.capacity(), [&]() { return rng(); });
//	std::sort(data.begin(), data.end(), std::greater<int>());
//}

void BulletManager::Fire(glm::vec2 pos, glm::vec2 dir, float speed, float time, float life_time)
{
	std::shared_ptr<Bullet> temp = std::make_shared<Bullet>(
		glm::vec3(pos, 0),
		glm::vec3(0, 0, 0),
		glm::vec3(5.0f, 5.0f, 5.0f)
		);

	temp->SetVelocity(speed);
	temp->SetTimeToSpawn(time);
	temp->SetLifeTime(life_time);

	temp->SetDirection(glm::vec3(dir,0.0f));

	temp->FaceDirection();

	temp->Initialize();

	BufferBullets.push_back(std::move(temp));
}

void BulletManager::GenerateWalls(float DeltaTime)
{
	if (WallMaxAmount < BufferWalls.size())
	{
		BufferWalls.erase(BufferWalls.begin() + WallMaxAmount, BufferWalls.end());
		return;
	}

	int currentMaxToSpawn = WallMaxAmount - BufferWalls.size();

	{
		float currentSpawn = (WallSpawnRate * WallSpawnAmount * DeltaTime);
		WallSpawnCache +=   currentSpawn - (float)((int)currentSpawn);
	}

	int amountToSpawnRightNow = (int)(WallSpawnRate * WallSpawnAmount * DeltaTime) + (int)WallSpawnCache;

	WallSpawnCache = WallSpawnCache - (float)((int)(WallSpawnCache));

	amountToSpawnRightNow = glm::clamp(amountToSpawnRightNow, 0, currentMaxToSpawn);

	for (int i = 0; i < amountToSpawnRightNow; ++i)
	{
		float x = HelpManager::randomf(10.0f, W_WIDTH - 10.0f);
		float y = HelpManager::randomf(10.0f, W_HEIGHT - 10.0f);
		float angle = HelpManager::randomf(0.0f, 360.0f);

		BufferWalls.push_back(
			std::make_shared<Wall>(
				glm::vec3(x, y, 0),
				glm::vec3(0, 0, angle),
				glm::vec3(10.0f, 2.0f, 1.0f)
				));

		BufferWalls.back()->Initialize();
	}
}

void BulletManager::GenerateBullets(float DeltaTime)
{
	if (BulletMaxAmount < BufferBullets.size())
	{
		BufferBullets.erase(BufferBullets.begin() + BulletMaxAmount, BufferBullets.end());
		return;
	}

	int currentMaxToSpawn = BulletMaxAmount - BufferBullets.size();


	//Cache needs 
	{
		float currentSpawn = (BulletSpawnRate * BulletSpawnAmount * DeltaTime);
		BulletSpawnCache += currentSpawn - (float)((int)currentSpawn);
	}

	int amountToSpawnRightNow = (int)(BulletSpawnRate * BulletSpawnAmount * DeltaTime) + (int)BulletSpawnCache;

	BulletSpawnCache = BulletSpawnCache - (float)((int)(BulletSpawnCache));

	amountToSpawnRightNow = glm::clamp(amountToSpawnRightNow, 0, currentMaxToSpawn);

	float x, y;
	float angle;
	for (int i = 0; i < amountToSpawnRightNow; ++i)
	{
		x = HelpManager::randomf(10.0f, (float)W_WIDTH - 10.0f);
		y = HelpManager::randomf(10.0f, (float)W_HEIGHT - 10.0f);

		Fire(
			glm::vec2(x, y),
			glm::vec2(HelpManager::randomf(-1.0f, 1.0f), HelpManager::randomf(-1.0f, 1.0f)),
			HelpManager::randomf(200.0f, 300.0f),
			HelpManager::randomf(1.0f, 10.0f),
			HelpManager::randomf(3.0f, 10.0f)
			);
	}
}

void BulletManager::DrawUI()
{
	if (ImGui::CollapsingHeader("Wall Spawn Properties",1))
	{
		ImGui::SliderInt("W: Max. Amount", &WallMaxAmount, 0, 1000);
		ImGui::SliderInt("W: Spawn amount", &WallSpawnAmount, 0, 1000);
		ImGui::SliderFloat("W: Spawn rate", &WallSpawnRate, 0.0f, 10.0f);
		if (ImGui::Button("W: Reset"))
		{
			WallMaxAmount = 100;
			WallSpawnRate = 1.0f;
			WallSpawnAmount = 10;
			WallSpawnCache = .0f;
		}
	}

	if (ImGui::CollapsingHeader("Bullet Spawn Prop.",2))
	{
		ImGui::SliderInt("B: Max. Amount", &BulletMaxAmount, 0, 1000);
		ImGui::SliderInt("B: Spawn amount", &BulletSpawnAmount, 0, 1000);
		ImGui::SliderFloat("B: Spawn rate", &BulletSpawnRate, 0.0f, 10.0f);
		if (ImGui::Button("B: Reset"))
		{
			BulletMaxAmount = 10;
			BulletSpawnRate = 1.0f;
			BulletSpawnAmount = 3;
			BulletSpawnCache = .0f;
		}
	}

	ImGui::Text("Walls  : %i", BufferWalls.size());
	ImGui::Text("Bullets: %i", BufferBullets.size());
}

void BulletManager::DrawObjects()
{
	for (size_t i = 0; i < BufferWalls.size(); ++i)
	{
		if (BufferWalls[i]->IsDestroyed() != true)
		{
			BufferWalls[i]->Draw();
		}
		else
		{
			BufferWalls.erase(BufferWalls.begin() + i);
			--i;
		}
	}
	for (size_t i = 0; i < BufferBullets.size(); ++i)
	{
		if (BufferBullets[i]->IsDestroyed() != true)
		{
			BufferBullets[i]->Draw();
		}
		else
		{
			BufferBullets.erase(BufferBullets.begin() + i);
			--i;
		}
	}
}

void BulletManager::CalculateCollisions(std::shared_ptr<Bullet>& singleBullet)
{
		if (singleBullet->IsDestroyed() == true)
			return;

	glm::vec3 originBullet = singleBullet->GetLocation();
	glm::vec3 scaleBullet = singleBullet->GetScale();
	SBounds boundsBullet = singleBullet->GetBounds();
	float angleBullet = glm::radians(singleBullet->GetRotation().z);

	bool bIsReflected = false;
	bool bIsRecalculated = true;

	//Find 4 points (bounds) of object basing on origin position and current rotation
	// to be able to find collision intersection
	std::vector<glm::vec3> sides = HelpManager::CalculateSides(originBullet, scaleBullet, boundsBullet.Width, boundsBullet.Height);

	for (size_t j = 0; j < sides.size(); ++j)
	{
		if (bIsReflected == true && bIsRecalculated == false) {
			sides = HelpManager::CalculateSides(originBullet, scaleBullet, boundsBullet.Width, boundsBullet.Height);
			bIsRecalculated = true;
		}

		sides[j] = HelpManager::RotateAroundPoint(originBullet, sides[j], angleBullet);

		//Calculate vertex bounds with window edges
		//For X axis
		if (sides[j].x < 0.0f || sides[j].x > W_WIDTH)
		{
			if (bIsReflected == true)
			{
				// Move bullet away from window edge
				float diff = glm::abs(sides[j].x);
				if (diff > W_WIDTH)
					diff = W_WIDTH - diff;

				glm::vec3 temp = originBullet;
				temp.x += diff;
				originBullet = temp;
				singleBullet->SetLocation(temp);

				bIsRecalculated = false;
			}
			else
			{
				//Reflect bullet from edge of the window and then
				// move object to eliminate intersection with window edge
				glm::vec3 Dir = singleBullet->GetDirection();
				Dir.x = -Dir.x;
				singleBullet->SetDirection(Dir);
				singleBullet->FaceDirection();
				angleBullet = glm::radians(singleBullet->GetRotation().z);
				bIsReflected = true;
				j = 0;
			}
		}
		//Same as before but for Y axis
		if (sides[j].y < 0.0f || sides[j].y > W_HEIGHT)
		{
			if (bIsReflected == true)
			{
				float diff = glm::abs(sides[j].y);
				if (diff > W_HEIGHT)
					diff = W_HEIGHT - diff;

				glm::vec3 temp = originBullet;
				temp.y += diff;
				originBullet = temp;
				singleBullet->SetLocation(temp);

				bIsRecalculated = false;
			}
			else
			{
				glm::vec3 Dir = singleBullet->GetDirection();
				Dir.y = -Dir.y;
				singleBullet->SetDirection(Dir);
				singleBullet->FaceDirection();
				angleBullet = glm::radians(singleBullet->GetRotation().z);
				bIsReflected = true;
				j = 0;
			}
		}

		if (j == 0)
			continue;

		//Bullet Line
		glm::vec3 A = sides[j - 1];
		glm::vec3 B = sides[j];

		//Basing on 2 closest bullet bounds and two bounds of wall, check if they intersect,
		//if yes, reflect them basing on wall rotation and bullet's direction vector
		for (size_t k = 0; k < BufferWalls.size(); ++k)
		{
			{
				//std::lock_guard<std::mutex> guard(mu1);

				if (BufferWalls[k]->IsDestroyed() == true)
					continue;
			}

			//Wall line
			glm::vec3 C;
			glm::vec3 D;

			float angleWall;
			glm::vec3 wallOrigin;
			SBounds wallBounds;

			{
				//std::lock_guard<std::mutex> guard(mu2);

				angleWall = glm::radians(BufferWalls[k]->GetRotation().z);
				wallOrigin = BufferWalls[k]->GetLocation();
				wallBounds = BufferWalls[k]->GetBounds();
			}

			C = glm::vec3(wallOrigin.x - wallBounds.Width / 2.0f, wallOrigin.y - wallBounds.Height / 2.0f, 0.0f);
			D = glm::vec3(wallOrigin.x + wallBounds.Width / 2.0f, wallOrigin.y - wallBounds.Height / 2.0f, 0.0f);

			C = HelpManager::RotateAroundPoint(wallOrigin, C, angleWall);
			D = HelpManager::RotateAroundPoint(wallOrigin, D, angleWall);


			bool bIsIntersect = HelpManager::intersect(A, B, C, D);
			//Reflect if objects are collided
			if (bIsIntersect == true)
			{
				float radians = angleWall + 1.57079633f;// rotate 90 degrees

				glm::vec3 wall = HelpManager::AngleToDirection(radians);

				//std::lock_guard<std::mutex> guard(mu3);

				glm::vec3 Dir = singleBullet->GetDirection();
				glm::vec3 reflected = glm::reflect(Dir, wall);
				singleBullet->SetDirection(reflected);
				singleBullet->FaceDirection();
				BufferWalls[k]->Destroy();
			}
		}
	}
}

