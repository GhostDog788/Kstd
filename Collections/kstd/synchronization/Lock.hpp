#pragma once
#include <kstd/memory/Move.hpp>

namespace kstd {

	class Lock
	{
	public:
		virtual ~Lock() = default;
		virtual void lock() = 0;
		virtual void unlock() = 0;
	};

	class LockGuard
	{
	public:
		LockGuard(Lock* lock) : m_lock(lock) {
			if (m_lock) m_lock->lock();
		};
		LockGuard(const LockGuard& other) = delete;
		LockGuard& operator=(LockGuard& other) = delete;
		LockGuard(LockGuard&& other) noexcept {
			m_lock = other.m_lock;
			other.m_lock = nullptr;
		}
		LockGuard& operator=(LockGuard&& other) noexcept {
			if (this != &other) {
				m_lock = other.m_lock;
				other.m_lock = nullptr;
			}
			return *this;
		}
		~LockGuard() {
			unlock();
		}
		void unlock() {
			if (m_lock) {
				m_lock->unlock();
				m_lock = nullptr;
			}
		}
		void clear() {
			m_lock = nullptr;
		}
	private:
		Lock* m_lock = nullptr;
	};
}