#pragma once
#include <kstd/memory/allocation.hpp>

namespace kstd {
	template<typename T>
	class DynamicBuffer
	{
	public:
		DynamicBuffer(size_t capacity = DEFAULT_NUM_OF_ITEMS, size_t expansion_size = DEFAULT_EXPANSION_SIZE) 
			: m_capacity(capacity), m_expansion_size(expansion_size), m_items(0) { m_buffer = allocate_array<T>(capacity); }
		DynamicBuffer(const DynamicBuffer&) = delete;
		DynamicBuffer& operator=(const DynamicBuffer&) = delete;
		DynamicBuffer(DynamicBuffer&& other) noexcept
		{
			_copy_from(other);
			other._moved();
		}
		DynamicBuffer& operator=(DynamicBuffer&& other) noexcept
		{
			if (this != &other)
			{
				_copy_from(other);
				other._moved();
			}
			return *this;
		}
		virtual ~DynamicBuffer() { free_array<T>(m_buffer); }

		DynamicBuffer& operator+=(const DynamicBuffer& other) {
			if (m_items + other.m_items > m_capacity)
			{
				resize(m_items + other.m_items);
			}
			for (size_t i = 0; i < other.m_items; ++i)
			{
				m_buffer[m_items + i] = other.m_buffer[i];
			}
			m_items += other.m_items;
			return *this;
		}

		virtual T& operator[](size_t index) const { return m_buffer[index]; }

		virtual size_t size() const { return m_items; }
		virtual size_t capacity() const { return m_capacity; }
		virtual size_t size_in_bytes() const { return m_items * sizeof(T); }
		virtual T* data() const { return m_buffer; }

		virtual void append(const T& item)
		{
			if (m_items >= m_capacity)
			{
				resize(m_capacity + m_expansion_size);
			}
			m_buffer[m_items] = item;
			m_items++;
		}

		virtual T& pop()
		{
			T& item = m_buffer[m_items - 1];
			m_items--;
			if (m_items <= m_capacity - m_expansion_size)
			{
				resize(m_capacity - m_expansion_size);
			}
			return item;
		}

		virtual void resize(size_t new_capacity)
		{
			m_capacity = new_capacity;
			T* new_buffer = allocate_array<T>(m_capacity);
			for (size_t i = 0; i < m_items; ++i)
			{
				new_buffer[i] = m_buffer[i];
			}
			free_array<T>(m_buffer);
			m_buffer = new_buffer;
		}
	protected:
		T* m_buffer = nullptr;
		size_t m_items = 0;
		size_t m_capacity = 0;
		size_t m_expansion_size = 0;

		virtual void _moved()
		{
			m_buffer = nullptr;
			m_items = 0;
			m_capacity = 0;
		}

		virtual void _copy_from(const DynamicBuffer& other)
		{
			m_buffer = other.m_buffer;
			m_items = other.m_items;
			m_capacity = other.m_capacity;
			m_expansion_size = other.m_expansion_size;
		}
		static constexpr auto DEFAULT_NUM_OF_ITEMS = 5;
		static constexpr auto DEFAULT_EXPANSION_SIZE = 5;
	};
	template <typename T>
	using vector = DynamicBuffer<T>;
}
