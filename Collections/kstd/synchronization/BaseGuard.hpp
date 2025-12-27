#pragma once
#include <kstd/memory/Move.hpp>

namespace kstd {

	template<typename T>
	class BaseGuard
	{
	public:
		BaseGuard(T& lock) : m_lock(&lock) {};
		BaseGuard(const BaseGuard& other) = delete;
		BaseGuard& operator=(BaseGuard& other) = delete;
		BaseGuard(BaseGuard&& other) {
			m_lock = other.m_lock;
			other.m_lock = nullptr;
		}
		BaseGuard& operator=(BaseGuard&& other) {
			if (this != &other) {
				m_lock = other.m_lock;
				other.m_lock = nullptr;
			}
			return *this;
		}
		virtual void clear() {
			m_lock = nullptr;
		}
	protected:
		T* m_lock = nullptr;

		virtual void _lock() = 0;
		virtual void _unlock() = 0;
	};
}