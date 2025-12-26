#pragma once
#include "DynamicBuffer.hpp"
#include <type_traits>

namespace kstd {
	size_t strlen(const char* s) {
		if (!s) return 0;
		size_t n = 0;
		while (s[n] != '\0') ++n;
		return n;
	}
	size_t wcslen(const wchar_t* s) {
		if (!s) return 0;
		size_t n = 0;
		while (s[n] != L'\0') ++n;
		return n;
	}

	template<typename T>
	class String : public DynamicBuffer<T>
	{
	static_assert(std::is_same_v<T, char> || std::is_same_v<T, wchar_t>, "String type must be char or wchar_t.");
	public:
		String() : DynamicBuffer<T>(this->DEFAULT_NUM_OF_ITEMS + 1) { this->m_capacity--; this->m_buffer[0] = 0; }
		String(const char* str) : DynamicBuffer<T>(strlen(str) + 1) { this->m_capacity--; copy_from(str); }
		String(const wchar_t* str) : DynamicBuffer<T>(wcslen(str) + 1) { this->m_capacity--; copy_from(str); }

		String& operator+=(const String& other) {
			DynamicBuffer<T>::operator+=(other);
			this->m_buffer[this->m_items] = end_char;
			return *this;
		}

		virtual void append(const T& item) override
		{
			DynamicBuffer<T>::append(item);
			this->m_buffer[this->m_items] = end_char;
		}

		virtual T& pop() override
		{
			T& item = DynamicBuffer<T>::pop();
			this->m_buffer[this->m_items] = end_char;
			return item;
		}

		virtual void resize(size_t new_capacity)
		{
			DynamicBuffer<T>::resize(new_capacity + 1);
			this->m_capacity--;
		}
	private:
		T end_char = 0;

		void copy_from(const T* str)
		{
			for (size_t i = 0; i < this->m_capacity; i++)
			{
				this->m_buffer[i] = str[i];
			}
			this->m_items = this->m_capacity;
			this->m_buffer[this->m_items] = end_char;
		}
	};
	using string = String<char>;
	using wstring = String<wchar_t>;
}
