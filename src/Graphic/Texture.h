#pragma once

#include <string>
#include "ErrorManager.h"

class Texture
{
private:
	unsigned int m_RendererID = 0;
	std::string m_FilePath = "";
	unsigned char* m_LocalBuffer = nullptr;
	int m_Width = -1, m_Height = -1, m_BPP = -1;
	
public:
	Texture() {};
	Texture(const std::string& path);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
};

