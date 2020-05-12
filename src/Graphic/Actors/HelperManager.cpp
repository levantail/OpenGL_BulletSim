#include "HelperManager.h"

bool HelpManager::first = true;

int HelpManager::randomi(int min, int max)
{
	if (first)
	{
		srand((unsigned int)time(NULL)); //seeding for the first time only!
		first = false;
	}
	return min + rand() % ((max + 1) - min);
}

float HelpManager::randomf(float min, float max)
{
	if (first)
	{
		srand((unsigned int)time(NULL)); //seeding for the first time only!
		first = false;
	}
	return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
}

std::vector<glm::vec3> HelpManager::CalculateSides(glm::vec3 origin, glm::vec3 scale, float width, float height)
{
	std::vector<glm::vec3> sides = {
		glm::vec3(origin.x + (width / 2.0f) , origin.y + (height / 2.0f) , 0.0f),
		glm::vec3(origin.x + (width / 2.0f) , origin.y - (height / 2.0f) , 0.0f),
		glm::vec3(origin.x - (width / 2.0f) , origin.y - (height / 2.0f) , 0.0f),
		glm::vec3(origin.x - (width / 2.0f) , origin.y + (height / 2.0f) , 0.0f)
	};
	return sides;

}
float HelpManager::EPS = 1E-9;

inline float HelpManager::det(float a, float b, float c, float d) {
	return a * d - b * c;
}

inline bool HelpManager::between(float a, float b, double c) {
	return std::min(a, b) <= c + EPS && c <= std::max(a, b) + EPS;
}

inline bool HelpManager::intersect_1(float a, float b, float c, float d) {
	if (a > b)  std::swap(a, b);
	if (c > d)  std::swap(c, d);
	return std::max(a, c) <= std::min(b, d);
}

bool HelpManager::intersect(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d) {
	float A1 = a.y - b.y, B1 = b.x - a.x, C1 = -A1 * a.x - B1 * a.y;
	float A2 = c.y - d.y, B2 = d.x - c.x, C2 = -A2 * c.x - B2 * c.y;
	float zn = det(A1, B1, A2, B2);
	if (zn != 0) {
		float x = -det(C1, B1, C2, B2) * 1.f / zn;
		float y = -det(A1, C1, A2, C2) * 1.f / zn;
		return between(a.x, b.x, x) && between(a.y, b.y, y)
			&& between(c.x, d.x, x) && between(c.y, d.y, y);
	}
	else
		return det(A1, C1, A2, C2) == 0 && det(B1, C1, B2, C2) == 0
		&& intersect_1(a.x, b.x, c.x, d.x)
		&& intersect_1(a.y, b.y, c.y, d.y);
}

glm::vec3 HelpManager::RotateAroundPoint(glm::vec3 Center, glm::vec3 PointToRotate, float radians)
{
	float s = glm::sin(radians);
	float c = glm::cos(radians);

	// translate point back to origin:
	PointToRotate.x -= Center.x;
	PointToRotate.y -= Center.y;

	// rotate point
	float xnew = PointToRotate.x * c - PointToRotate.y * s;
	float ynew = PointToRotate.x * s + PointToRotate.y * c;

	// translate point back:
	PointToRotate.x = xnew + Center.x;
	PointToRotate.y = ynew + Center.y;

	return PointToRotate;
}

glm::vec3 HelpManager::AngleToDirection(float radians)
{
	return glm::vec3(glm::cos(radians), glm::sin(radians), 0.0f);
}
