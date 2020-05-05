#include "Actor.h"
#include <algorithm>

Actor::Actor(glm::vec3 Location, glm::vec3 Rotation, glm::vec3 Scale) :
	Transform({
		Location,
		Rotation,
		Scale})
{
}

void Actor::Draw()
{
	if (isInitilized == false)
	{
		isInitilized = true;
		Initialize();
	}

	glm::quat myquaternion = glm::quat(glm::radians(Transform.Rotation));

	glm::mat4 TranslationMatrix = translate(glm::mat4(1.0f), Transform.Location);
	glm::mat4 RotationMatrix = glm::mat4_cast(myquaternion);
	glm::mat4 ScaleMatrix = scale(glm::mat4(1.0f), Transform.Scale);

	glm::mat4 ModelMatrix = TranslationMatrix * RotationMatrix * ScaleMatrix;

	glm::mat4 MVP = ProjectionMat * VievMat * ModelMatrix;

	shader->Bind();
	shader->SetUniformMat4f("u_MVP", MVP);
	
	Renderer::Draw(*va, *ib, *shader);
	

	Transform_PrevFrame = Transform;
}

void Actor::Initialize()
{
	// Open GL data Bind to the GPU
	vb =  std::make_unique<VertexBuffer>(Vertices.data(), Vertices.size() * sizeof(float));
	va = std::make_unique<VertexArray>();
	layout = std::make_unique<VertexBufferLayout>();

	layout->Push<float>(2);
	layout->Push<float>(2);
	va->AddBuffert(*vb, *layout);

	ib = std::make_unique<IndexBuffer>(Indices.data(), Indices.size());


	shader = std::make_unique<Shader>("res/shaders/Basic.shader");
	shader->Bind();

	texture = std::make_unique<Texture> ("res/textures/test.png");
	texture->Bind();
	//shader->SetUniform1i("u_Texture", 0);

	shader->Unbind();
	va->Unbind();
	vb->Unbind();
	ib->Unbind();

	// Object bounds calculation
	// First 2 floats stands for x and y vertices respectively
	// both seconds - Texture pos S and T
	float MaxX = FLT_MIN;
	float MinX = FLT_MAX;

	float MaxY = FLT_MIN;
	float MinY = FLT_MAX;

	for (size_t i = 0; i < Vertices.size(); i += 4)
	{
		//x
		MaxX = std::max(Vertices[i], MaxX);
		MinX = std::min(Vertices[i], MinX);
		//y
		MaxY = std::max(Vertices[i + 1], MaxY);
		MinY = std::min(Vertices[i + 1], MinY);
	}

	//Absolute difference;
	Bounds.Width = abs(MaxX - MinX) * Transform.Scale.x;
	Bounds.Height = abs(MaxY - MinY) * Transform.Scale.y;
}
