#pragma once
#include <time.h>
#include <cstdlib>
#include <vector>
#include "glm/glm.hpp"
#include <glm\detail\type_vec3.hpp>
#include <algorithm>

class HelpManager
{
public:

	static int randomi(int min, int max);

	static float randomf(float min, float max);

	static std::vector<glm::vec3> CalculateSides(glm::vec3 origin, glm::vec3 scale, float width, float height);
	static bool intersect(const glm::vec3 a, const glm::vec3 b, const glm::vec3 c, const glm::vec3 d);
	static glm::vec3 RotateAroundPoint(const glm::vec3 Center, const glm::vec3 PointToRotate, const float radians);
	static glm::vec3 AngleToDirection(const float radians);
private:
	static bool first;
	HelpManager() {};

	
	static float EPS;
	static inline float det(float a, float b, float c, float d);
	static inline bool between(float a, float b, double c);

	static inline bool intersect_1(float a, float b, float c, float d);
	
};