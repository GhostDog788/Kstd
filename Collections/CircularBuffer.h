#pragma once

template<typename T>
class CircularBuffer
{
public:
	CircularBuffer(size_t max_items) : m_max_items(max_items) { m_buffer = new T[max_items]; }
	virtual ~CircularBuffer() { delete[] m_buffer; }

	void push(const T& item)
	{
		size_t prev_head = m_head;
		m_head = (m_head + 1) % m_max_items;
		if (prev_head != -1 && prev_head > m_head) m_head_overflowed = true;
		m_buffer[m_head] = item;
	}

	T& operator[](size_t index) const { return m_buffer[index]; }

	bool head(T& head) const {
		if (m_head == -1) return false;
		head = m_buffer[m_head];
		return true;
	}

	bool at(size_t offset_from_head, T& item) const {
		if (offset_from_head >= m_max_items || (m_head == -1)) return false;
		if (!m_head_overflowed && offset_from_head > m_head) return false;
		int num_head = m_head;
		if ((int)(m_head - offset_from_head) < 0) num_head += m_max_items;
		item = m_buffer[(num_head - offset_from_head) % m_max_items];
		return true;
	}

	size_t size() const { return m_max_items; }

private:
	T* m_buffer = nullptr;
	size_t m_head = -1;
	bool m_head_overflowed = false;
	size_t m_max_items = 0;
};
