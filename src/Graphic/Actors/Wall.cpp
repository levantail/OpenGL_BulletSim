#include "Wall.h"

Wall::Wall(glm::vec3 Location, glm::vec3 Rotation, glm::vec3 Scale) : Actor (Location, Rotation, Scale)
{
	Vertices =
	{
		//	X		Y	  S		T
		 3.0f,  .3f,	1.0f, 1.0f,
		 3.0f, -.3f,	1.0f, 0.0f,
		-3.0f, -.3f,	0.0f, 0.0f,
		-3.0f,  .3f,	0.0f, 1.0f
	};

	Indices =
	{
		0, 1, 2,
		2, 3, 0 
	};
}
