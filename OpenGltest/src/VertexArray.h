#pragma once
#include "VertexBuffer.h"
#include "VertextBufferLayout.h"

class VertexArray
{
private:
	unsigned int m_renderId;
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertextBufferLayout& layout);
	void Bind() const;
	void UnBind() const;
};

