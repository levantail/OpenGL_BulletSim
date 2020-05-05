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
	static bool intersect(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d);
	static glm::vec3 RotateAroundPoint(glm::vec3 Center, glm::vec3 PointToRotate, float radians);
	static glm::vec3 AngleToDirection(float radians);
private:
	static bool first;
	HelpManager() {};

	
	static double EPS;
	static inline float det(float a, float b, float c, float d);
	static inline bool between(float a, float b, double c);

	static inline bool intersect_1(float a, float b, float c, float d);
	
};