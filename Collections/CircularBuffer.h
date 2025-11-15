#pragma once
#include "StaticBuffer.h"

template<typename T>
class CircularBuffer : public StaticBuffer<T>
{
public:
	CircularBuffer(size_t max_items) : StaticBuffer<T>(max_items) {}

	void push(const T& item)
	{
		size_t prev_head = m_head;
		m_head = (m_head + 1) % this->m_items;
		if (prev_head != -1 && prev_head > m_head) m_head_overflowed = true;
		this->m_buffer[m_head] = item;
	}

	bool head(T& head) const {
		if (m_head == -1) return false;
		head = this->m_buffer[m_head];
		return true;
	}

	bool at(size_t offset_from_head, T& item) const {
		if (offset_from_head >= this->m_items || (m_head == -1)) return false;
		if (!m_head_overflowed && offset_from_head > m_head) return false;
		int num_head = (int)m_head;
		if ((int)(m_head - offset_from_head) < 0) num_head += (int)(this->m_items);
		item = this->m_buffer[(num_head - offset_from_head) % this->m_items];
		return true;
	}

private:
	size_t m_head = -1;
	bool m_head_overflowed = false;
};
