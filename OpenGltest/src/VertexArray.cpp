#include "VertexArray.h"
#include "VertextBufferLayout.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_renderId));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_renderId));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertextBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*) offset));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_renderId));
}

void VertexArray::UnBind() const
{
	GLCall(glBindVertexArray(0));
}
