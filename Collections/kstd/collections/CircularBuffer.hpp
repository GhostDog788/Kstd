#pragma once
#include "StaticBuffer.hpp"

namespace kstd {

	template<typename T, unsigned int SIZE>
	class CircularBuffer : public StaticBuffer<T, SIZE>
	{
	public:
		CircularBuffer(bool override_oldest = false) : StaticBuffer<T, SIZE>(), m_override_oldest(override_oldest) {}
		virtual ~CircularBuffer() = default;

		virtual bool push(const T& item)
		{
			this->m_buffer[m_write] = item;
			m_write = (m_write + 1) % SIZE;
			if (m_write == m_read) {
				if (!m_override_oldest) return false;
				m_read = (m_read + 1) % SIZE;
			}
			return true;
		}

		virtual bool head(T& head) const {
			if (m_read == m_write) return false;
			head = this->m_buffer[m_read];
			return true;
		}
		virtual bool pop(T& head) {
			if (m_read == m_write) return false; // Buffer is empty
			head = this->m_buffer[m_read];
			m_read = (m_read + 1) % SIZE;
			return true;
		}

	private:
		size_t m_read = 0;
		size_t m_write = 0;
		bool m_override_oldest = false;
	};
}
