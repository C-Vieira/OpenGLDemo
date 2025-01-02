#pragma once

class VertexBuffer {
private:
	unsigned int m_RendererID;
public:
	VertexBuffer(const void* data, unsigned int size);
	VertexBuffer(unsigned int capacity);
	~VertexBuffer();

	void Bind() const;
	void UnBind() const;
};