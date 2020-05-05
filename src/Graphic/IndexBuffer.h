#pragma once

class IndexBuffer
{
private:
	unsigned int m_RendererID;
	unsigned int m_Count; 
public:
	IndexBuffer() { m_RendererID = 0; m_Count = 0; };
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();


	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount()const { return m_Count; }
};