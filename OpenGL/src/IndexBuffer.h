#pragma once

class IndexBuffer
{
private:
	unsigned int m_RendererID;
	unsigned int m_Count;

public:
	IndexBuffer(const unsigned int* data=0, unsigned int count=0);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return m_Count; };
};