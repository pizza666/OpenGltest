#pragma once
class IndexBuffer
{
private:
	unsigned int m_renderId;
	unsigned int m_Count;
public:
	IndexBuffer(const unsigned int* indizes, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return m_Count; }
};

