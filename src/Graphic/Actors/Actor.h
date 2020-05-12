#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

#include "../ErrorManager.h"
#include "../VertexBuffer.h"
#include "../IndexBuffer.h"
#include "../VertexArray.h"
#include "../Shader.h"
#include "../Renderer.h"
#include "../Texture.h"
#include "../DeltaTime.h"
//Math Library
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"

struct STransform
{
public:
	glm::vec3 Location;
	glm::vec3 Rotation;
	glm::vec3 Scale;
};

struct SBounds
{
	float Height;
	float Width;
};

class Actor
{
public:
	Actor(	glm::vec3 Location =	glm::vec3(0.0f,0.0f,0.0f),
			glm::vec3 Rotation =	glm::vec3(0.0f,0.0f,0.0f),
			glm::vec3 Scale    =	glm::vec3(1.0f,1.0f,1.0f));

	//Called every frame (same as update)
	virtual void Tick(float DeltaTime);
	//Drawing procedure.
	virtual void Draw();
	//Bind resources to memory and prepare for drawing.
	virtual void Initialize();

	//Event on collision
	virtual void OnCollisionAABB(Actor actor) {};

	//Destroy current object.
	virtual void Destroy() { OnDestroy(); bIsWaitngToDelete = true;/*delete this;*/ };

	inline virtual bool IsDestroyed() { return bIsWaitngToDelete; };


	//Setters Getters

	inline virtual STransform GetTransform() { return this->Transform; };
	inline virtual STransform GetTransform_PrevFrame() { return this->Transform_PrevFrame; };

	inline virtual glm::vec3 GetLocation() { return this->Transform.Location; };
	inline virtual glm::vec3 GetRotation() { return this->Transform.Rotation; };
	inline virtual glm::vec3 GetScale() { return this->Transform.Scale; };

	inline virtual SBounds GetBounds() { return this->Bounds; };

	inline virtual glm::vec3 GetDirection() { return this->Direction; };

	virtual void SetDirection(glm::vec3 newDirection) { this->Direction = newDirection; };

	virtual void SetRotation(glm::vec3 newRotation) { this->Transform.Rotation = newRotation; };
	virtual void SetLocation(glm::vec3 newLocation) { this->Transform.Location = newLocation; };
protected:
	float Velocity = 10.0f;

	//Normalized Values i.e. from -1 to 1 included.
	glm::vec3 Direction = glm::vec3(1.0f, 1.0f, 0.0f);

	//Current Transform
	STransform Transform;
	//Pref frame transform for position fix in case of overlapping solid object
	STransform Transform_PrevFrame;

	bool bIsWaitngToDelete = false;

	//All things that should be happen before actual object destroy should be here.
	virtual void OnDestroy() {};

	glm::mat4 MVP = glm::mat4(1.0f);
	glm::mat4 ProjectionMat = glm::ortho(0.f, 1280.f, 0.f, 820.0f, -1.0f, 1.0f);
	glm::mat4 VievMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 0.f));

	//Opengl wrappers for binding buffers and further 'drawing'
	std::unique_ptr <VertexArray> va;
	std::unique_ptr <VertexBufferLayout> layout;
	std::unique_ptr <VertexBuffer> vb;
	std::unique_ptr <IndexBuffer> ib;

	std::unique_ptr<Shader> shader;
	std::unique_ptr<Texture> texture;

	bool isInitilized = false;

	// 'Mesh' vertices & Texture positions:
	std::vector<float> Vertices = 
	{
	//	X		Y	  S		T
		-5.0f, -5.0f, 0.0f, 0.0f,
		-5.0f,  5.0f, 0.0f, 1.0f,
		 5.0f,  5.0f, 1.0f, 1.0f,
		 5.0f, -5.0f, 1.0f, 0.0f
	};

	// Sequence of how the object will be drawn
	std::vector<unsigned int> Indices = 
	{
		0, 1, 2,
		0, 2, 3
	};

	// Object bounds, i.e. total width and height. Generated basing on Vertices.
	SBounds Bounds;
};