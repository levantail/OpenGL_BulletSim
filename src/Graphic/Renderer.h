#pragma once
#include "ErrorManager.h"
#include "VertexArray.h"
#include "Shader.h"
#include "IndexBuffer.h"

class Renderer
{
private:
public:
	static void Clear();
	static void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);

};