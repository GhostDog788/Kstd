#pragma once

namespace kstd {

	template<typename T>
	class StaticBuffer
	{
	public:
		StaticBuffer(size_t items) : m_items(items) { m_buffer = new T[items]; }
		StaticBuffer(const StaticBuffer&) = delete;
		StaticBuffer& operator=(const StaticBuffer&) = delete;
		StaticBuffer(StaticBuffer&& other) noexcept
		{
			_copy_from(other);
			other._moved();
		}
		StaticBuffer& operator=(StaticBuffer&& other) noexcept
		{
			if (this != &other)
			{
				_copy_from(other);
				other._moved();
			}
			return *this;
		}
		virtual ~StaticBuffer() { delete[] m_buffer; }

		T& operator[](size_t index) const { return m_buffer[index]; }

		size_t size() const { return m_items; }
		size_t size_in_bytes() const { return m_items * sizeof(T); }

	protected:
		T* m_buffer = nullptr;
		size_t m_items = 0;

		void _moved()
		{
			m_buffer = nullptr;
			m_items = 0;
		}

		void _copy_from(const StaticBuffer& other)
		{
			m_buffer = other.m_buffer;
			m_items = other.m_items;
		}
	};
}
