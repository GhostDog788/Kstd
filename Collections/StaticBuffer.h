#pragma once

template<typename T>
class StaticBuffer
{
public:
	StaticBuffer(size_t items) : m_items(items) { m_buffer = new T[items]; }
	virtual ~StaticBuffer() { delete[] m_buffer; }

	T& operator[](size_t index) const { return m_buffer[index]; }

	size_t size() const { return m_items; }
	size_t size_in_bytes() const { return m_items * sizeof(T); }

protected:
	T* m_buffer = nullptr;
	size_t m_items = 0;
};